# Architecture Rules

Apply the shared [architecture-governance skill](https://github.com/cshaxu/skills/blob/main/architecture-governance/SKILL.md).
The concrete SoftPC ownership map is [System Architecture](../design/ARCHITECTURE.md).

- The recovered SoftPC machine has one owner: the pristine core. CPU, C-VID,
  controllers, BIOS, ROM, BOP semantics, guest RAM, and device state do not
  acquire a second implementation in host or frontend code.
- The standalone host owns only host capabilities: execution orchestration,
  timer delivery, media I/O, input collection, renderer surfaces, audio, and
  process/window integration.
- The runtime has exactly one executor that may mutate SoftPC state. Frontends
  communicate through bounded commands/input and consume copied frame snapshots.
- Public cross-module interfaces use opaque handles and copied values; they do
  not expose raw CPU, RAM, controller, renderer, or executor state.
- A compatibility adapter replaces an original host boundary; it must not
  reinterpret a device protocol, BIOS service, BOP selector, or guest media.
- Transitional adapters state their owner, scope, removal condition, and a
  verification that prevents a second production route.
