# M9 T56 S9 closure: monitor output spacing

## Outcome

The owner reopened T56 because `Unknown command.` was immediately followed by
the next prompt. The product monitor contract now states one rule: every
nonempty command/result text ends in `\r\n\r\n`; a pure Enter emits no text and
only rearms the prompt.

`src/app/command.c` has one shared formatter used by parser rejections,
lifecycle completed facts, and local floppy results. Help already had the same
trailing blank-line shape and remains unchanged. No command acceptance,
lifecycle request, Console handoff, MVDM, lib API, or product configuration
semantics changed.

## Verification

- The command smoke now proves spacing for unknown/rejected commands, local
  floppy results, lifecycle completed results, and the empty-line exception.
- Fresh full CTest: x64 **62/62**, x86 **62/62**.
- Strict standalone lib CTest: **8/8**; source-boundary and documentation
  governance gates passed.
- Fresh fixed package SHA-256:
  - `softpc32.exe`: `EC898D950BE62FF34D34A47A4BE77BCF8E0CE1926C6714A387E4CCD0F43015E7`
  - `softpc64.exe`: `B2F8CFF5C37F56D63AF83EC11377E69E0397C91EA4AC0FE5F5E50EBEF2BFC1ED`

T56 is closed again with a clean worktree. Every future S remains required to
rebuild and test both fixed-width package executables.
