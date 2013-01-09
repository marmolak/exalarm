#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include "exalarm.h"

static struct sigaction sa;
static struct sigaction old_sa;
static int ex_initialized = 0;

static void ex_sig_handler (int signum, siginfo_t *siginfo, void *blank);

static void ex_sig_handler (int signum, siginfo_t *siginfo, void *blank)
{
	(void) blank;
	
	/* checks */
	if ( ex_initialized == 0 ) { return; }

	/* don't raise exception if we receive signal from outer world. */
#ifdef DEBUG
	(void) siginfo;
#else
	if (siginfo->si_pid != 0) { return; }
#endif

	switch (signum) {
                // timeout exception
		case SIGALRM:
			raise_ex (timeout_ex);
			return;
			break;

		default:
			return;
			break;
        }
}

/* check exalarm.h for do_exception macro */
void init_exception (int const* type_test)
{
	(void) type_test; /* only for strict type check */

	assert (ex_initialized == 0);
	ex_initialized = 1;
}

inline int set_alarm (int timeout)
{
	if (timeout == 0) {
		ex_initialized = 0;
		return 0;
	}

	assert (ex_initialized == 1);
		
	memset (&sa, 0, sizeof (sa));
	sa.sa_sigaction = &ex_sig_handler;
	sa.sa_flags = SA_SIGINFO;

	sigset_t oset;
	sigprocmask (SIG_BLOCK, NULL, &oset);
	sa.sa_mask = oset;

	int ret = sigaction (SIGALRM, &sa, &old_sa);
	if ( ret == -1 ) { return ret; }

	alarm (timeout);
	return ret;
}

inline void disable_alarm (void)
{
	if ( ex_initialized == 0 ) { return; }

	ex_initialized = 0;
	alarm (0);
	sigaction (SIGALRM, &old_sa, NULL);
}

inline void raise_ex (exceptions_t exnum)
{
	assert (ex_initialized == 1);

	alarm (0);
	ex_initialized = 0;

	sigaction (SIGALRM, &old_sa, NULL);

	siglongjmp (ex_buf, exnum);
}
