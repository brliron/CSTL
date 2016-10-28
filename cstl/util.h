#ifndef UTIL_

# define	for_each(type, it, v, block)	\
  do {						\
    if (v->size == 0)				\
      break ;					\
    int	_pos = 0;				\
    type	it = v->data[_pos];		\
    while (_pos < v->size)			\
      {						\
	block;					\
	_pos++;					\
	it = v->data[_pos];			\
      }						\
  } while (0)

# define	TOKENPASTE(x, y)	x ## y
# define	TOKENPASTE2(x, y)	TOKENPASTE(x, y)

#endif /* UTIL_ */
