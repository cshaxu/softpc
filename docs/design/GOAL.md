# Project Goal

Deliver one independent, direct-launch SoftPC virtual machine. It preserves
the selected original SoftPC machine shape and its BIOS/VGA ROM/BOP device
semantics while removing NTVDM, DOS/WOW, BaseSrv, CSR, VDD, and product-session
dependencies.

The product has one VM instance, no profile picker, no command-line options,
and one adjacent `softpc.ini` that selects memory, A:/C: images, media mode,
and console/window presentation. It must remain usable through local and RDP
input without replacing SoftPC devices or renderers.
