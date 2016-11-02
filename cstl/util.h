#ifndef UTIL_
# define UTIL_

# if defined(__GNUC__)
#  define CSTL_UNUSED __attribute__((unused))
# else
#  define CSTL_UNUSED
# endif

# if defined(__i386__) || defined(_M_IX86)
#   define CSTL_FUNC_CALL_SIZE	23
#   define CSTL_FUNC_CALL_DEF(name, ...)	name(MKNAME(,)* this, void* ret_addr, ##__VA_ARGS__)
#   define CSTL_FUNC_CALL_PRE			(void)ret_addr;
#  elif defined(__x86_64__) || defined(_M_X64)
#   if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#    define CSTL_FUNC_CALL_SIZE	31
#   else /* WINDOWS */
#    define CSTL_FUNC_CALL_SIZE	37
#   endif /* WINDOWS */
#   define CSTL_FUNC_CALL_DEF(name, ...)	name(MKNAME(,)* this, ##__VA_ARGS__)
#   define CSTL_FUNC_CALL_PRE
# endif

void*		gen_func_call(void* this, unsigned char* buff, void* func);
/*
** Note: the x86_64 Windows version supports only 3 additional parameters, and the x86_64 Unix version supports only 5 additional parameters.
*/

# include	<string.h>

# define	for_each(type, it, o, block)			\
  do {								\
    cstl_iterator*	_iterator = o->begin();			\
    while (_iterator != NULL)					\
      {								\
	type	it;						\
	memcpy(&it, &_iterator->data, _iterator->data_size);	\
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
# pragma pack(push, 1)
typedef struct			_cstl_iterator
{
  unsigned char			_next_jmp[CSTL_FUNC_CALL_SIZE];
  struct _cstl_iterator*	(*next)();
  int				data_size;
  char				data;
}				cstl_iterator;
# pragma pack(pop)

#endif /* UTIL_ */
