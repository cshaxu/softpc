#!/usr/bin/env python3
"""Emit the standalone CMOS port-ABI overlay from pristine controller source."""

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
        '#include "config.h"\n',
        '#include "config.h"\n#include <cmos/softpc_cmos_host.h>\n',
        "host-port include",
    )
    # The original host encoded its graphics scalar in a pointer.  Preserve
    # the controller expression while making that historical ABI explicit on
    # both x86 and x64 hosts.
    source = replace_once(
        source,
        'adapter = (ULONG) config_inquire(C_GFX_ADAPTER, NULL);',
        'adapter = (ULONG)(ULONG_PTR) config_inquire(C_GFX_ADAPTER, NULL);',
        "graphics scalar port",
    )
    source = replace_once(
        source,
        '''\t/* Check the Fixed Disk Type */
\t disk = 0x30;         /* Drive C type always 3 - then <<4 */
\t /* check whether D drive exists */
\t if ( *((CHAR *) config_inquire(C_HARD_DISK2_NAME, NULL)))
\t\t disk = 0x34;         /* 3 << 4 | 4 */
''',
        '''\t/* Check the Fixed Disk Type through the standalone media port. */
\t disk = softpc_host_cmos_fixed_disk_type();
''',
        "fixed-media port",
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
