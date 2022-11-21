#include "libtty.h"

#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "liberr.h"

const char* SERIAL_VID = "10C4";
const char* SERIAL_PID = "EA60";

const char* ESPPRG_VID = "1A86";
const char* ESPPRG_PID = "7523";

const char* cmd = "powershell.exe";
const char* arg = "(Get-WmiObject -class Win32_PnPEntity | Where-Object {$_.DeviceID -like \"*VID_%s&PID_%s*\"}).Name";

const char* search = "(COM";

// sets up powershell command to be run with execvp
// pipes must be set up before calling this function
static void execute_powershell( void ) {
	// string substitution
	char sub_arg[1024];
	snprintf( sub_arg, 1024, arg, SERIAL_VID, SERIAL_PID, ESPPRG_VID, ESPPRG_PID );

	// creating argv
	char* argv[3] = { (char*)cmd, sub_arg, 0 };

	// execute
	execvp( argv[0], argv );
}

// returns fd of pipe that corresponds to output of powershell command
static int get_output( void ) {
	int fds[2];
	if ( pipe( fds ) < 0 ) panic( "pipe failed: %s\n", strerror( errno ) );

	int pid = fork();
	if ( pid < 0 ) panic( "fork failed: %s\n", strerror( errno ) );
	if ( pid == 0 ) {
		dup2( fds[1], 1 );
		close( fds[0] );
		close( fds[1] );

		execute_powershell();
	}

	close( fds[1] );

	waitpid( pid, NULL, 0 );

	return fds[0];
}

int* find_ttyusb_list( void ) {
	int fd = get_output();

	char buf[1024];

	size_t n = read( fd, buf, sizeof( buf ) - 1 );
	buf[n] = 0;

	printf( "%s", buf );

	int ret_tmp[32];
	char* p = buf;
	int i = 0;
	while ( ( p = strstr( p, search ) ) ) {
		p += 4;
		ret_tmp[i] = atoi( p );
		i++;
	}

	int* ret = malloc( sizeof( int ) * ( i + 1 ) );
	ret[i] = -1;

	memcpy( ret, ret_tmp, sizeof( int ) * i );

	return ret;
}

char* find_ttyusb( void ) {
	int* list = find_ttyusb_list();

	if ( list[0] == -1 ) panic( "found zero devices\n" );
	if ( list[1] != -1 ) panic( "found more than 1 device\n" );

	char buf[1024];
	snprintf( buf, sizeof( buf ), "/dev/ttyS%d", list[0] );

	return strdup( buf );
}

static int compare_timespec( struct timespec* a, struct timespec* b ) {
	if ( a->tv_sec != b->tv_sec ) return a->tv_sec - b->tv_sec;
	if ( a->tv_nsec != b->tv_nsec ) return a->tv_nsec - b->tv_nsec;
	return 0;
}

static void set_timespec( struct timespec* dst, struct timespec* src ) {
	dst->tv_sec = src->tv_sec;
	dst->tv_nsec = src->tv_nsec;
}

char* find_ttyusb_last( void ) {
	int* list = find_ttyusb_list();
	if ( list[0] == -1 ) panic( "found zero devices\n" );

	char buf[1024];
	struct stat s;

	snprintf( buf, sizeof( buf ), "/dev/ttyS%d", list[0] );
	stat( buf, &s );

	struct timespec latest_time;
	int latest_dev = list[0];
	set_timespec( &latest_time, &s.st_mtim );

	for ( int i = 1; list[i] != -1; i++ ) {
		snprintf( buf, sizeof( buf ), "/dev/ttyS%d", list[i] );
		stat( buf, &s );

		if ( compare_timespec( &latest_time, &s.st_mtim ) > 0 ) {
			set_timespec( &latest_time, &s.st_mtim );
			latest_dev = list[i];
		}
	}

	snprintf( buf, sizeof( buf ), "/dev/ttyS%d", latest_dev );
	return strdup( buf );
}

char* find_ttyusb_first( void ) {
	int* list = find_ttyusb_list();
	if ( list[0] == -1 ) panic( "found zero devices\n" );

	char buf[1024];
	struct stat s;

	snprintf( buf, sizeof( buf ), "/dev/ttyS%d", list[0] );
	stat( buf, &s );

	struct timespec oldest_time;
	int oldest_dev = list[0];
	set_timespec( &oldest_time, &s.st_mtim );

	for ( int i = 1; list[i] != -1; i++ ) {
		snprintf( buf, sizeof( buf ), "/dev/ttyS%d", list[i] );
		stat( buf, &s );

		if ( compare_timespec( &oldest_time, &s.st_mtim ) < 0 ) {
			set_timespec( &oldest_time, &s.st_mtim );
			oldest_dev = list[i];
		}
	}

	snprintf( buf, sizeof( buf ), "/dev/ttyS%d", oldest_dev );
	return strdup( buf );
}
