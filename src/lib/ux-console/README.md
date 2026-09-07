# ux-console

`ux-console` depends on `ux-base` and `base` only. It owns one VM Console lifecycle,
creates its logical Console object, and publishes copied text frames through
that object. It never opens, registers, or renders native Console I/O.
