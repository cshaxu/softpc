#!/usr/bin/env python3
"""Rewrite C-VID generated GDP dereferences for a native-width host.

The original CPU_40 generator emits a 32-bit ``Gdp + offset`` ABI.  The
ordinary C sources already use the source-derived GDP side table; this tool
extends the same storage contract to generated ``sevid*.c`` rules without
modifying those source inputs.  It writes only the requested build output.
"""

from __future__ import annotations

import argparse
import pathlib
import re


STATIC_GDP = re.compile(
    r"\*\s*\(\s*\(\s*(?P<type>IUH)\s*\*\s*\)\s*"
    r"\(\s*r1\s*\+\s*(?P<offset>\d+)\s*\)\s*\)"
)

# Generated EDL emits this form when the original source selected a GDP field
# through a temporary rule register: *((TYPE *)((*((IHPE *)&r1)) + rNN)).
DYNAMIC_GDP = re.compile(
    r"\*\s*\(\s*\(\s*(?P<type>IUH|IU32|IU16|IU8|IHPE)\s*\*\s*\)\s*"
    r"\(\s*\(\s*\*\s*\(\s*\(\s*IHPE\s*\*\s*\)\s*"
    r"&\s*\(\s*r1\s*\)\s*\)\s*\)\s*\+\s*"
    r"\*\s*\(\s*\(\s*IHPE\s*\*\s*\)\s*"
    r"&\s*\(\s*(?P<offset_reg>r\d+)\s*\)\s*\)\s*\)\s*\)"
)

# A second generated form first places a historical GDP byte offset in a rule
# register, then dereferences that register directly.  At runtime the helper
# distinguishes these small legacy offsets from native pointers.
INDIRECT_GDP = re.compile(
    r"\*\s*\(\s*\(\s*(?P<type>IUH|IU32|IU16|IU8|IHPE)\s*\*\s*\)\s*"
    r"\(\s*\*\s*\(\s*\(\s*IHPE\s*\*\s*\)\s*"
    r"&\s*\(\s*(?P<address_reg>r\d+)\s*\)\s*\)\s*\)\s*\)"
)

# The generated translator measures this private stack in 32-bit IUH bytes.
# Its indexed accesses are IUH slots, so preserve the source slot count while
# allocating native-width storage on a 64-bit host.  Restrict this to the
# generated LocalIUH allocation; other malloc calls retain byte semantics.
LOCAL_IUH_MALLOC = re.compile(
    r"(?P<prefix>LocalIUH\s*=\s*\(\s*IUH\s*\*\s*\)\s*malloc\s*\(\s*)"
    r"(?P<bytes>\d+)(?P<suffix>\s*\))"
)


def static_replacement(match: re.Match[str]) -> str:
    return (
        "*((%s *)softpc_gdp_rule_slot((void *)r1, %su, sizeof(%s)))"
        % (match.group("type"), match.group("offset"), match.group("type"))
    )


def dynamic_replacement(match: re.Match[str]) -> str:
    value_type = match.group("type")
    offset_reg = match.group("offset_reg")
    return (
        "*((%s *)softpc_gdp_rule_slot((void *)r1, "
        "(unsigned int)(*((IHPE *)&(%s))), sizeof(%s)))"
        % (value_type, offset_reg, value_type)
    )


def indirect_replacement(match: re.Match[str]) -> str:
    value_type = match.group("type")
    address_reg = match.group("address_reg")
    return (
        "*((%s *)softpc_gdp_rule_address((void *)r1, "
        "(uintptr_t)(*((IHPE *)&(%s))), sizeof(%s)))"
        % (value_type, address_reg, value_type)
    )


def local_iuh_malloc_replacement(match: re.Match[str]) -> str:
    byte_count = int(match.group("bytes"))
    if byte_count % 4 != 0:
        raise ValueError("generated LocalIUH allocation is not word aligned")
    # Generated rules write 32-bit subregisters into these IUH slots and later
    # reload the full slot.  Zero initialization supplies the required x64
    # zero-extension for the otherwise unwritten upper half.
    return "%s%du, sizeof(IUH)%s" % (
        match.group("prefix").replace("malloc", "calloc"),
        byte_count // 4,
        match.group("suffix"),
    )


def transform(source: str) -> tuple[str, int, int]:
    source, dynamic_count = DYNAMIC_GDP.subn(dynamic_replacement, source)
    source, static_count = STATIC_GDP.subn(static_replacement, source)
    source, _ = INDIRECT_GDP.subn(indirect_replacement, source)
    source, _ = LOCAL_IUH_MALLOC.subn(local_iuh_malloc_replacement, source)
    marker = '#include "softpc_gdp_rule_access.h"\n'
    if marker not in source:
        first_include_end = source.find("\n", source.find("#include")) + 1
        if first_include_end <= 0:
            raise ValueError("generated source has no include insertion point")
        source = source[:first_include_end] + marker + source[first_include_end:]
    return source, static_count, dynamic_count


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    parser.add_argument("--report", type=pathlib.Path)
    args = parser.parse_args()

    original = args.source.read_text(encoding="latin-1")
    generated, static_count, dynamic_count = transform(original)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(generated, encoding="latin-1", newline="")
    if args.report is not None:
        args.report.write_text(
            "%s static=%d dynamic=%d\n"
            % (args.source.name, static_count, dynamic_count),
            encoding="ascii",
        )
    if static_count == 0 and dynamic_count == 0:
        raise ValueError("no recognized GDP dereference in " + str(args.source))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
