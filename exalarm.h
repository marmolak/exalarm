#ifndef _EXALARM_H
#define _EXALARM_H

#include <setjmp.h>

enum exceptions {
	timeout_ex = 2,
} exceptions_t;

inline int set_alarm (int timeout);
inline void disable_alarm (void);
void init_exception (int *type_test);

sigjmp_buf ex_buf;

#define  do_exception(x) do { init_exception ((x)); *(x) = sigsetjmp (ex_buf, 1); } while (0)

#endif
