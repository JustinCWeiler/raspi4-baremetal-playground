#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#include "libserial.h"
#include "liberr.h"
#include "libcrc.h"
#include "bootdefs.h"

static const uint8_t test_prog[] = {
	0x01, 0xc4, 0xbf, 0x52, 0x00, 0x80, 0x80, 0x52, 0x01, 0x04, 0x80, 0x72,
	0x20, 0x00, 0x00, 0xb9, 0x02, 0x02, 0xa0, 0x52, 0x42, 0x04, 0x00, 0x71,
	0xe1, 0xff, 0xff, 0x54, 0x81, 0x05, 0x80, 0x72, 0x20, 0x00, 0x00, 0xb9,
	0x02, 0x02, 0xa0, 0x52, 0x42, 0x04, 0x00, 0x71, 0xe1, 0xff, 0xff, 0x54,
	0xf4, 0xff, 0xff, 0x17
};

static const uint32_t nbytes = sizeof(test_prog);

uint8_t read_byte(int fd) {
	uint8_t data;
	int ret;
	if ((ret = read(fd, &data, 1)) == 1)
		return data;

	if (ret < 0)
		die("Error reading serial: %s\n", strerror(errno));
	else
		die("Connection to serial closed\n");
}

void write_byte(int fd, uint8_t b) {
	if (write(fd, &b, 1) < 1)
		die("Error writing serial: %s\n", strerror(errno));
}

int main(void) {
	int fd = get_ttyusb();

	uint32_t crc = crc32(test_prog, nbytes);

	printf("----------1----------\n");

	// read GET_INFO
	while (read_byte(fd) != GET_INFO) ;

	printf("----------2----------\n");

	// write PUT_INFO
	write_byte(fd, PUT_INFO);

	// write nbytes
	for (int i = 0; i < 32; i += 8)
		write_byte(fd, nbytes >> i);

	// write crc
	for (int i = 0; i < 32; i += 8)
		write_byte(fd, crc >> i);

	printf("----------3----------\n");

	// drain buffer
	uint8_t recv;
	while ( (recv = read_byte(fd)) == GET_INFO) ;

	printf("----------4----------\n");

	// read GET_CODE
	if (recv != GET_CODE)
		die("Expected GET_CODE, got 0x%02x\n", recv);

	printf("----------5----------\n");

	// read crc
	for (int i = 0; i < 32; i += 8) {
		recv = read_byte(fd);
		if (recv != ((crc >> i) & 0xff))
			die("Expected crc byte 0x%02x and got 0x%02x\n", (crc >> i) & 0xff, recv);
	}

	printf("----------6----------\n");

	// write PUT_CODE
	write_byte(fd, PUT_CODE);

	// write program bytes
	for (int i = 0; i < nbytes; i++) {
		write_byte(fd, test_prog[i]);
	}

	printf("----------7----------\n");

	// read SUCCESS
	recv = read_byte(fd);
	if (recv != SUCCESS)
		die("Expected SUCCESS, got 0x%02x\n", recv);

	printf("----------8----------\n");

	printf("Successfully booted!\n");
}
