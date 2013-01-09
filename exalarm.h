#ifndef _EXALARM_H
#define _EXALARM_H

#include <setjmp.h>

#ifndef __GNUC__
#  define  __attribute__(x)  /*NOTHING*/
#endif

typedef enum exceptions {
	timeout_ex = 2,
} exceptions_t;


inline int set_alarm (int timeout);
inline void disable_alarm (void);
void init_exception (int const* type_test) __attribute__((__nonnull__(1)));
inline void raise_ex (exceptions_t exnum);

sigjmp_buf ex_buf;

#define  do_exception(x) do { init_exception ((x)); *(x) = sigsetjmp (ex_buf, 1); } while (0)

#endif
