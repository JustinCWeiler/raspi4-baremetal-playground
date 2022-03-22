#define MK_FN(sym) .global sym; sym:

#define PUSH(r0, r1) stp r0, r1, [sp, #-16]!
#define POP(r0, r1) ldp r0, r1, [sp], #16

#define MB dsb sy
#define MB_RD dsb ld
#define MB_WR dsb st
