# Documentation Rules

Apply the shared [documentation-governance skill](https://github.com/cshaxu/skills/blob/main/documentation-governance/SKILL.md).
This document adds the SoftPC-specific topology and authority boundaries.

`docs/README.md` is the sole entry point. Its direct directories are exactly
`rules/`, `design/`, `states/`, `proposals/`, `history/`, and `etc/`. `rules/`
contains exactly this document plus `EXECUTION.md`, `ARCHITECTURE.md`, and
`CODING.md`; `design/` contains exactly `GOAL.md`, `ARCHITECTURE.md`,
`CODING.md`, `UI.md`, and `ROADMAP.md`.

| Location | Owns | Does not own |
| --- | --- | --- |
| `rules/` | Enforceable constraints. | Concrete product design or task state. |
| `design/` | Current goal, component/source/UI design, milestones. | Rule copies or active task contracts. |
| `states/CURRENT.md` | One active packet, current baseline, recent closure summaries. | Long task narratives or queue detail. |
| `states/QUEUE.md` | Ordered unnumbered candidate links. | Active work or version allocation. |
| `states/TODO.md` | Deferred debt and admission trigger. | Hidden queue work or completed facts. |
| `proposals/` | Unnumbered candidate context. | Current authority after admission. |
| `history/` | Closed numeric task facts and retained proposals. | Current rules, design, or status. |
| `etc/` | Indexed evidence/supporting detail. | Competing rules, design, queue, or state. |

One subject has one current authority; link rather than copy. Root `README.md`,
`AGENTS.md`, and `CONTRIBUTING.md` are pointers only. `etc/README.md` indexes
every supporting subtree with owner, purpose, and promotion/retirement rule.
Historical terminology remains historical. Do not place local media paths,
guest data, generated traces, or build outputs in tracked documentation.
