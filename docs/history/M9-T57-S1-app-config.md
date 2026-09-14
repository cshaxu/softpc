# M9 T57 S1: App Configuration Extraction

Owner admits the app/vm/compat refactor, with only main allowed to know VM,
and no Lib/Common/MVDM modifications. Full original request and frozen
ownership ledger: [proposal](../proposals/m9-app-vm-compat-boundaries.md).
Baseline cfc88cf; configuration extraction is the first bounded step.

Moved the existing parser and path helpers, preserving record splitting,
comments/quotes, permitted keys, error returns, default values and path rules.
Config uses Common display and Lib media types; main explicitly converts to
the old machine options until S2 replaces that composition boundary.
No second parser, compatibility forwarding function or new runtime exists.

Similar-issue sweep covered all config functions and CMake main-source reuse:
package and two source-list-based runtime tests compile the same config.c.
Focused config smoke covers comments, quoted values, media/display/control,
invalid memory/key/display and relative/absolute/empty paths using a copied
in-memory storage fixture, never product INI/media. No shared tests modified.

Source accounting: main +8/-189; config C/H +194/-0; net production C/H +13.
Test adds 59 lines; CMake adds the package source and test target. This is a
move with one small contract, not a parser rewrite. Existing TODOs unchanged.

Verification: both fixed EXEs built; x64 full suite 84/84 (76.37 s).
x86 full suite 84/84 (97.33 s). Documentation gate passes; protected five corpora
have zero diff. Final acceptance and actual-commit review follow the complete
dual-width delivery.

Actual-commit review of 393f4a0: parser body is moved rather than rewritten;
new display/media types are converted explicitly at composition; production
and fake-input assertions preserve behavior. Both package suites passed.
Creating this history record made the old New/S1 packet stale; the final
documentation check rejected it, although the shell continued to commit.
S2 admission now advances the packet and reruns the gate, without weakening
the verifier. S1 is accepted; no runtime defect was hidden by this correction.
