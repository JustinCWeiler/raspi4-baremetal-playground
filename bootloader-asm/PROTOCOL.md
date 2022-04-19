# Protocol

The protocol we use is based off the protocol used for Stanford's CS 140E class.
This one is however slightly simplified, mainly in that it uses 8-bit protocol
values as opposed to 32-bit. The original protocol is specified
[here](https://github.com/dddrrreee/cs140e-22win/blob/main/labs/3-bootloader/README.md).

We use 8-bit protocol values for two main reasons:

1. It can be loaded with a single mov instruction,
   removing the need to ldr values from some location.
2. The UART sends data one byte at a time.

The issue with this is that there's a higher chance of collision with real data,
but we ignore that because for practical purposes collisions won't happen.

Here is an example exchange:

```txt
    PI                     UNIX
GET_INFO
              ----->
                       PUT_INFO
                       nbytes[7:0]
                       nbytes[15:8]
                       nbytes[23:16]
                       nbytes[31:24]
                       crc32[7:0]
                       crc32[15:8]
                       crc32[23:16]
                       crc32[31:24]
              <-----
GET_CODE
crc32[7:0]
crc32[15:8]
crc32[23:16]
crc32[31:24]
              ----->
                       PUT_CODE
                       send code bytes
              <-----
SUCCESS
              ----->
```
