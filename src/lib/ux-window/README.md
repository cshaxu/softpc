# ux-window

`ux-window` depends on `ux-base` and `base` only. It owns one Window lifecycle and sends
copied UX events to the application queue entry. It never includes `host` or
makes product decisions.

The Window owns only host presentation mechanics: it draws a guest text cursor
from copied position/shape/enabled frame fields and toggles that drawing every
250 ms while unfrozen. `freeze()` atomically prevents capture, releases any
capture, and holds the cursor at its current drawn state; `unfreeze()` resumes
the blink but waits for a later client-area click before it captures. Native
VM Console cursor blinking remains outside this component.

The public component contract is cross-platform. This corpus currently has a
supported Win32 implementation only; the Linux leaf is an intentional
`LIB_STATUS_UNSUPPORTED` placeholder, not a claimed Linux presenter.
