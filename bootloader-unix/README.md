# Bootloader-Unix

The UNIX side of the bootloader for the Pi 4. Unlike bootloader-asm, this will
not be written in "pure, beautiful" assembly, because while I am masochistic,
I am not insane.

This program essentially finds a serial tty device, opens a file descriptor,
sets up the correct config for the protocol we want (8n1), and does the
byte exchange.
