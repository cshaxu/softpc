# ux-window

`ux-window` depends on `ux-base` and `base` only. It owns one Window lifecycle and sends
copied UX events to the application queue entry. It never includes `host` or
makes product decisions.
The public component contract is cross-platform. This corpus currently has a
supported Win32 implementation only; the Linux leaf is an intentional
`LIB_STATUS_UNSUPPORTED` placeholder, not a claimed Linux presenter.
