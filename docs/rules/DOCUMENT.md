# Documentation Rules

`docs/README.md` is the sole entry point. Its direct directories are exactly
`rules/`, `design/`, `states/`, `proposals/`, `history/`, and `etc/`.

One subject has one current authority. `CURRENT.md` owns active work and the
technical baseline; Queue owns only ordering; proposals own candidate detail;
history owns closed numbered-task evidence. `etc/README.md` indexes supporting
material and cannot define a competing plan.

Documentation changes must preserve this division and validate all local links.
Do not place local media paths, guest data, generated traces, or build outputs
in tracked documentation.
