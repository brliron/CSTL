#ifndef UTIL_
# define UTIL_

# include "gen_func_call.h"

# include	<string.h>

# define	for_each(type, it, o, block)			\
  do {								\
    cstl_iterator*	_iterator = o->begin();			\
    while (_iterator != NULL)					\
      {								\
	type	it;						\
	memcpy(&it, _iterator->data, _iterator->data_size);	\
	block;							\
	_iterator = _iterator->next();				\
      }								\
  } while (0)

# define	TOKENPASTE(x, y)	x ## y
# define	TOKENPASTE2(x, y)	TOKENPASTE(x, y)

/*
** Note about iterators:
** For efficiency, and to avoid the user to free them in some cases,
** iterators won't be malloc'ed, it will be stored in the object.
** As a result, the user won't be able to use 2 iterators for the same
** object at the same time.
*/
typedef struct			_cstl_iterator
{
  unsigned char			_next_jmp[CSTL_FUNC_CALL_SIZE];
  struct _cstl_iterator*	(*next)();
  int				data_size;
  char				data[1];
}				cstl_iterator;

#endif /* UTIL_ */
