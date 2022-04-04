# Bootloader-Asm

Bootloader over UART written in pure, beautiful assembly.
We (almost definitely) don't get any performance boost by writing all our own
assembly, but it's a "fun" exercise.
To be honest, it's probably gonna be pretty bad but hey good experience.

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
