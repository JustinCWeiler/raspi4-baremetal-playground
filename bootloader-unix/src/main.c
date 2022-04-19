#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#include "libserial.h"
#include "liberr.h"
#include "libcrc.h"
#include "libfile.h"
#include "bootdefs.h"

uint8_t read_byte(int fd) {
	uint8_t data;
	int ret;
	if ((ret = read(fd, &data, 1)) == 1)
		return data;

	if (ret < 0)
		panic("Error reading serial: %s\n", strerror(errno));
	else
		panic("Connection to serial closed\n");
}

void write_byte(int fd, uint8_t b) {
	if (write(fd, &b, 1) < 1)
		panic("Error writing serial: %s\n", strerror(errno));
}

int main(int argc, const char** argv) {
	int tty_fd;
	uint8_t* prog;
	uint32_t nbytes;
	if (argc < 2)
		die("Usage: %s [-last|-first|/dev/ttySX] <program.bin>\n", argv[0]);
	else if (argc < 3) {
		nbytes = read_file(argv[1], &prog);
		tty_fd = get_ttyusb();
	}
	else if (argc < 4) {
		nbytes = read_file(argv[2], &prog);
		if (strcmp("-last", argv[1]) == 0)
			tty_fd = get_ttyusb_last();
		else if (strcmp("-first", argv[1]) == 0)
			tty_fd = get_ttyusb_first();
		else
			tty_fd = get_usb(argv[1]);
	}
	else
		die("Error: too many arguments");

	uint32_t crc = crc32(prog, nbytes);

	// read GET_INFO
	while (read_byte(tty_fd) != GET_INFO) ;

	// write PUT_INFO
	write_byte(tty_fd, PUT_INFO);

	// write nbytes
	for (int i = 0; i < 32; i += 8)
		write_byte(tty_fd, nbytes >> i);

	// write crc
	for (int i = 0; i < 32; i += 8)
		write_byte(tty_fd, crc >> i);

	// drain buffer
	uint8_t recv;
	while ( (recv = read_byte(tty_fd)) == GET_INFO) ;

	// read GET_CODE
	if (recv != GET_CODE)
		panic("Expected GET_CODE, got 0x%02x\n", recv);

	// read crc
	for (int i = 0; i < 32; i += 8) {
		recv = read_byte(tty_fd);
		if (recv != ((crc >> i) & 0xff))
			panic("Expected crc byte 0x%02x and got 0x%02x\n", (crc >> i) & 0xff, recv);
	}

	// write PUT_CODE
	write_byte(tty_fd, PUT_CODE);

	// write program bytes
	for (int i = 0; i < nbytes; i++) {
		write_byte(tty_fd, prog[i]);
	}

	// read SUCCESS
	recv = read_byte(tty_fd);
	if (recv != SUCCESS)
		panic("Expected SUCCESS, got 0x%02x\n", recv);

	printf("Successfully booted!\n");
}
