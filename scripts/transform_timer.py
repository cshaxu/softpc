#!/usr/bin/env python3
"""Emit the standalone timer port-ABI overlay from pristine controller source."""

from __future__ import annotations

import argparse
import pathlib


ORIGINAL = '''\t\t\t\t\ttimer_generate_int(1);
\t\t\t\t\ttimer_multiple_delay = SYSTEM_TICK_INTV / n;
\t\t\t\t\tactive_int_event = TRUE;
'''

PORT_ABI = '''\t\t\t\t\ttimer_generate_int(1);
\t\t\t\t\ttimer_multiple_delay = SYSTEM_TICK_INTV / n;
\t\t\t\t\t/* The standalone host must defer a zero-delay queue entry by
\t\t\t\t\t   one microsecond: the original queue's immediate-event path
\t\t\t\t\t   otherwise recursively drains a valid one-clock PIT backlog. */
\t\t\t\t\tif (timer_multiple_delay == 0)
\t\t\t\t\t\ttimer_multiple_delay = 1;
\t\t\t\t\tactive_int_event = TRUE;
'''


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    args = parser.parse_args()
    source = args.source.read_text(encoding="latin-1")
    if source.count(ORIGINAL) != 1:
        raise ValueError("expected one timer quick-event port replacement")
    generated = source.replace(ORIGINAL, PORT_ABI, 1)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if (not args.output.exists() or
            args.output.read_text(encoding="latin-1") != generated):
        args.output.write_text(generated, encoding="latin-1", newline="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
