# Protocol

The protocol we use is the one used by the CS 140E class at Stanford.
This is so I can be lazy and use their provided `pi-install` program.
(Note: for the C bootloader I'll make later, I'll make my own protocol).

The protocol is copied here for ease of access, with slight modifications.
You can view the original description [here](https://github.com/dddrrreee/cs140e-22win/blob/main/labs/3-bootloader/README.md).

## The bootloader protocol

At a high level, the protocol works as follows: Here you'll write
implement the code to:

  1. pi: spin in a loop, periodically asking for the program to run;
  2. unix: send the program code;
  3. pi: receive the program code, copy it to where it should go, and
     jump to it.
  4. To detect corruption, we use a checksum (a hash) that we can
     compare the received data too.

This is a stripped down version (good enough for out purposes):

    =======================================================
            pi side             |               unix side
    -------------------------------------------------------
     put_uint(GET_PROG_INFO)+ ----->

                                     put_uint(PUT_PROG_INFO);
                                     put_uint(ARMBASE);
                                     put_uint(nbytes);
                            <------- put_uint(crc32(code));

     put_uint32(GET_CODE)
     put_uint32(crc32)      ------->
                                     <check crc = the crc value sent>
                                     put_uint(PUT_CODE);
                                     foreach b in code
                                          put_byte(b);
                             <-------
    <copy code to addr>
    <check code crc32>
    put_uint(BOOT_SUCCESS)
                             ------->
                                      <done!>
                                      start echoing any pi output to 
                                      the terminal.
    =======================================================

Note that `put_uint` is 4 bytes sent in little-endian order.

More descriptively:

  1. The pi will repeatedly signal it is ready to receive the program by
     sending `GET_PROG_INFO` requests.

  2. When the unix side receives a `GET_PROG_INFO`, it sends back
     `PUT_PROG_INFO` along with: the constant to load the code at (ARMBASE),
     the size of the code in bytes, and a CRC  of the code.

  3. The pi-side checks the code address and the size, and if OK (i.e.,
     it does not collide with its own currently running code) it sends a
     `GET_CODE` request along with the CRC value it received in step
     2 (so the server can check it). Otherwise it sends an error
     `BAD_CODE_ADDR` and reboots.

  4. The unix side sends `PUT_CODE` and the code.

  5. The pi side copies the received code into memory starting at the
     indicated code address (from step 2).  It then computes
     `crc32(addr,nbytes)` over this range and compares it to the
     expected value received in step 2.  If they do not match, it sends
     `BAD_CODE_CKSUM`. If they do match, it sends back `BOOT_SUCCESS`.

  6. Once the Unix side receives `BOOT_SUCCESS` it simply echoes all
     subsequent received bytes to the terminal.

  7. If at any point the pi side receives an unexpected message, it
     sends a `BOOT_ERROR` message to the unix side and reboots.
     If at any point the Unix side receives an unexpected or error
     message it simply exits with an error.

The use of send-response in the protocol is intended to prevent the
Unix-side from overrunning the pi sides finite-sized UART queue.  The CRC
and other checks guard against corruption.
