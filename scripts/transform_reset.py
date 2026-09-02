#!/usr/bin/env python3
"""Emit the standalone BIOS-reset host ABI overlay from pristine source."""

from __future__ import annotations

import argparse
import pathlib


def replace_once(source: str, old: str, new: str, description: str) -> str:
    if source.count(old) != 1:
        raise ValueError("expected one %s replacement" % description)
    return source.replace(old, new, 1)


def transform(source: str) -> str:
    source = replace_once(
        source,
        '#ifdef NTVDM\n        half_word cmos_diskette;\n#endif',
        '#if defined(NTVDM) || defined(SOFTPC_STANDALONE)\n'
        '        half_word cmos_diskette;\n#endif',
        "CMOS declaration",
    )
    source = replace_once(
        source,
        '#ifdef NTVDM\n\tequip_flag.bits.diskette_present = FALSE;',
        '#if defined(NTVDM) || defined(SOFTPC_STANDALONE)\n'
        '\t/* Standalone equipment is the attached FDC topology, not the\n'
        '\t   original product default that always exposed a floppy. */\n'
        '\tequip_flag.bits.diskette_present = FALSE;',
        "CMOS topology branch",
    )
    # config_inquire encodes the original graphics scalar as a pointer.  Keep
    # the reset algorithm intact while making this host ABI explicit on x64.
    source = replace_once(
        source,
        'if((ULONG) config_inquire(C_GFX_ADAPTER, NULL) == CGA )',
        'if((ULONG_PTR) config_inquire(C_GFX_ADAPTER, NULL) == CGA )',
        "graphics comparison",
    )
    source = replace_once(
        source,
        'switch((ULONG)config_inquire(C_GFX_ADAPTER, NULL))',
        'switch((ULONG_PTR)config_inquire(C_GFX_ADAPTER, NULL))',
        "graphics switch",
    )
    source = replace_once(
        source,
        'gfxAdapt = (ULONG)config_inquire(C_GFX_ADAPTER, NULL);',
        'gfxAdapt = (SHORT)(ULONG_PTR)config_inquire(C_GFX_ADAPTER, NULL);',
        "graphics assignment",
    )
    return source


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    args = parser.parse_args()
    generated = transform(args.source.read_text(encoding="latin-1"))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if (not args.output.exists() or
            args.output.read_text(encoding="latin-1") != generated):
        args.output.write_text(generated, encoding="latin-1", newline="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
