#include	<stdlib.h>
#include	"gen_func_call.h"

#if !defined(CSTL_TYPE) || !defined(CSTL_NAME)
# error you muse define CSTL_TYPE and CSTL_NAME.
#endif

#include	"util.h"

#define		MKNAME(begin, end)	TOKENPASTE2(TOKENPASTE2(begin ## list, CSTL_NAME), end)


MKNAME(struct _,);

typedef				MKNAME(struct _,_node)
{
  CSTL_TYPE			data;
  MKNAME(struct _,)*		parent;
  MKNAME(struct _,_node)*	prev;
  MKNAME(struct _,_node)*	next;
}				MKNAME(,_node);

typedef			MKNAME(struct _,)
{
  MKNAME(,_node)*	begin;
  MKNAME(,_node)*	end;
  int			size;

  unsigned char		_push_front_jmp[26];
  void			(__stdcall *push_front)(CSTL_TYPE val);
  unsigned char		_push_back_jmp[26];
  void			(__stdcall *push_back)(CSTL_TYPE val);
  unsigned char		_delete_jmp[26];
  void			(__stdcall *delete)();
}			MKNAME(,);

static void __stdcall	MKNAME(,_push_front)(MKNAME(,)* l, CSTL_TYPE val)
{
  MKNAME(,_node)*	node;

  node = malloc(sizeof(MKNAME(,_node)));
  node->data = val;
  node->parent = l;
  node->prev = NULL;
  node->next = l->begin;
  if (l->begin)
    l->begin->prev = node;
  l->begin = node;
  if (l->end == NULL)
    l->end = node;
  l->size++;
}

static void __stdcall	MKNAME(,_push_back)(MKNAME(,)* l, CSTL_TYPE val)
{
  MKNAME(,_node)*	node;

  node = malloc(sizeof(MKNAME(,_node)));
  node->data = val;
  node->parent = l;
  node->prev = l->end;
  node->next = NULL;
  if (l->end)
    l->end->next = node;
  if (l->begin == NULL)
    l->begin = node;
  l->end = node;
  l->size++;
}

static void __stdcall	MKNAME(,_delete)(MKNAME(,)* l)
{
  MKNAME(,_node)*	node;
  MKNAME(,_node)*	next;

  node = l->begin;
  while (node)
    {
      next = node->next;
      free(node);
      node = next;
    }
  free(l);
}

__attribute__((unused)) static MKNAME(,)*	MKNAME(,_new)()
{
  MKNAME(,)*	l;

  l = malloc(sizeof(MKNAME(,)));
  l->begin = NULL;
  l->end = NULL;
  l->size = 0;
  l->push_front	= gen_func_call(l, l->_push_front_jmp,	MKNAME(,_push_front));
  l->push_back	= gen_func_call(l, l->_push_back_jmp,	MKNAME(,_push_back));
  l->delete	= gen_func_call(l, l->_delete_jmp,	MKNAME(,_delete));
  return l;
}


#undef CSTL_TYPE
#undef CSTL_NAME
#undef MKNAME
