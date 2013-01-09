#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "exalarm.h"

int main (int argc, char **argv)
{
	(void) argc; (void) argv;

	int exc = 0;
	do_exception (&exc);
	if ( ! exc ) {

		set_alarm (1);
		sleep (10);
		disable_alarm ();
	} else {
		disable_alarm ();
		if ( exc == timeout_ex ) {
			printf ("timeout!\n");
		} else if ( exc ) {
			printf ("error code: %d\n", exc);
		}
	}
	disable_alarm ();

	return EXIT_SUCCESS;
}
