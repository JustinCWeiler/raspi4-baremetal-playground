# Bootloader-C

Bootloader now written in C. The idea of this part is to minimize as much as
possible and squeeze as much speed out of the bootloader as possible. Also,
this bootloader will most likely be more correct than the assembly one simply
due to the fact that it's written in C.

Functionally, the bootloader will work the same, so it should be hard to tell
from the outside whether a Pi has the assembly or C version of the bootloader.

The functionality is copied here from the asm README for ease of access:

The bootloader starts by copying the main code into the top region of memory
so that it can be used again without having to reboot. This is because the
boot time on the Pi 4 is way too long to deal with for rapid testing, and
so not having to reboot every time is super useful.

This does come with 2 major drawbacks:

1. We cannot set the stack to the end of memory.
2. We will have residual machine state between programs.

The first issue is fine because I will explicitly know this when making
programs. Two possible solutions are setting the stack to 0x80000,
and setting the stack to right below the bootloader program. The first is
good because it's very very simple to do, but comes with the drawback that
we have around 512 KiB of stack memory. This shouldn't be much of an issue,
but it is something to consider. The second is good because we get a lot more
stack memory, but with the cost of it being hard to compute where the stack
goes. We would also have to worry about stack alignment.

The second issue is addressed in the fact that device initialization is
*usually* idempotent, and when it's not there's usually an error. Any other
case is unlikely, so I won't worry about it.
