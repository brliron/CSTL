#include	<stdlib.h>
#include	"gen_func_call.h"

#if !defined(CSTL_TYPE) || !defined(CSTL_NAME)
# error you muse define CSTL_TYPE and CSTL_NAME.
#endif

#include	"util.h"

#define		MKNAME(begin, end)	TOKENPASTE2(TOKENPASTE2(begin ## vector, CSTL_NAME), end)


typedef		MKNAME(struct _,)
{
  CSTL_TYPE*	data;
  int		size;

  unsigned char	_push_back_jmp[26];
  void		(__stdcall *push_back)(CSTL_TYPE val);
}		MKNAME(,);

static void __stdcall	MKNAME(,_push_back)(MKNAME(,)* v, CSTL_TYPE val)
{
  if (v->size % 10 == 0)
    v->data = realloc(v->data, (v->size + 10) * sizeof(CSTL_TYPE));
  v->data[v->size] = val;
  v->size++;
}

__attribute__((unused)) static MKNAME(,)*	MKNAME(,_new)()
{
  MKNAME(,)*	v;

  v = malloc(sizeof(MKNAME(,)));
  v->data = NULL;
  v->size = 0;
  v->push_back = gen_func_call(v, v->_push_back_jmp, MKNAME(, _push_back));
  return v;
}


#undef CSTL_TYPE
#undef CSTL_NAME
#undef MKNAME
