#include	<stdlib.h>
#include	"gen_func_call.h"

#include	"util.h"

#ifndef VECTOR_H_
# define VECTOR_H_

struct			vector_base
{
  void*			data;
  int			size;
  int			data_size;

  cstl_iterator		it;
  // I can't add new fields after this one because the size of the generic
  // iterator may be different from the original one.
};

void			vector_generic_push_back(void* this_, void* val);

#endif /* !VECTOR_H_ */




#ifndef CSTL_DONT_DEFINE_TYPE

# if !defined(CSTL_TYPE) || !defined(CSTL_NAME)
#  error you muse define CSTL_TYPE and CSTL_NAME.
# endif

# define		MKNAME(begin, end)	TOKENPASTE2(TOKENPASTE2(begin ## vector, CSTL_NAME), end)


typedef		MKNAME(struct _,)
{
  CSTL_TYPE*	data;
  int		size;
  int		data_size;

  unsigned char	_push_back_jmp[26];
  void		(__stdcall *push_back)(CSTL_TYPE val);
}		MKNAME(,);

static void __stdcall	MKNAME(,_push_back)(MKNAME(,)* this, CSTL_TYPE val)
{
  vector_generic_push_back(this, &val);
}

__attribute__((unused)) static MKNAME(,)*	MKNAME(,_new)()
{
  MKNAME(,)*	v;

  v = malloc(sizeof(MKNAME(,)));
  v->data = NULL;
  v->size = 0;
  v->data_size = sizeof(CSTL_TYPE);
  v->push_back = gen_func_call(v, v->_push_back_jmp, MKNAME(, _push_back));
  return v;
}


# undef CSTL_TYPE
# undef CSTL_NAME
# undef MKNAME

#endif /* !CSTL_DONT_DEFINE_TYPE */
