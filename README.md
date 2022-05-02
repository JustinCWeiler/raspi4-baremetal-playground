# raspi4-baremetal-playground

Random programs made to run on a baremetal Raspberry Pi 4.
May become a simple OS repo, we'll see.

The folder `aarch64-none-elf` (ignored by git) contains the required build tools
for 64-bit Arm baremetal development. To set up, follow the steps below:

1. Download the tar with this command.
`wget https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf.tar.xz`
2. Extract the tar in the root folder of this repository.
3. Rename the resulting folder to `aarch64-none-elf`

Make sure to use the above version (11.2-2022.02), otherwise filepaths will differ.
