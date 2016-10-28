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

typedef				MKNAME(struct _,_iterator)
{
  unsigned char			_next_jmp[26];
  struct _cstl_iterator*	(_stdcall *next)();
  int				data_size;
  CSTL_TYPE			data;
  MKNAME(,_node)*		node;
}				MKNAME(,_iterator);

static cstl_iterator*	MKNAME(,_next)(MKNAME(,_iterator)* it)
{
  if (it->node->next == NULL)
    return NULL;
  it->node = it->node->next;
  it->data = it->node->data;
  return (cstl_iterator*)it;
}



typedef			MKNAME(struct _,)
{
  MKNAME(,_node)*	front;
  MKNAME(,_node)*	back;
  int			size;
  
  MKNAME(,_iterator)	it;

  unsigned char		_push_front_jmp[26];
  void			(__stdcall *push_front)(CSTL_TYPE val);
  unsigned char		_push_back_jmp[26];
  void			(__stdcall *push_back)(CSTL_TYPE val);
  unsigned char		_begin_jmp[26];
  cstl_iterator*	(__stdcall *begin)();
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
  node->next = l->front;
  if (l->front)
    l->front->prev = node;
  l->front = node;
  if (l->back == NULL)
    l->back = node;
  l->size++;
}

static void __stdcall	MKNAME(,_push_back)(MKNAME(,)* l, CSTL_TYPE val)
{
  MKNAME(,_node)*	node;

  node = malloc(sizeof(MKNAME(,_node)));
  node->data = val;
  node->parent = l;
  node->prev = l->back;
  node->next = NULL;
  if (l->back)
    l->back->next = node;
  if (l->front == NULL)
    l->front = node;
  l->back = node;
  l->size++;
}

static cstl_iterator*	MKNAME(,_begin)(MKNAME(,)* l)
{
  if (l->front == NULL)
    return NULL;
  l->it.node = l->front;
  l->it.data = l->front->data;
  return (cstl_iterator*)&l->it;
}

static void __stdcall	MKNAME(,_delete)(MKNAME(,)* l)
{
  MKNAME(,_node)*	node;
  MKNAME(,_node)*	next;

  node = l->front;
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
  l->front = NULL;
  l->back = NULL;
  l->size = 0;
  l->push_front	= gen_func_call(l, l->_push_front_jmp,	MKNAME(,_push_front));
  l->push_back	= gen_func_call(l, l->_push_back_jmp,	MKNAME(,_push_back));
  l->begin	= gen_func_call(l, l->_begin_jmp,	MKNAME(,_begin));
  l->delete	= gen_func_call(l, l->_delete_jmp,	MKNAME(,_delete));
  
  l->it.data_size = sizeof(CSTL_TYPE);
  l->it.next	= gen_func_call(&l->it, l->it._next_jmp, MKNAME(,_next));
  return l;
}


#undef CSTL_TYPE
#undef CSTL_NAME
#undef MKNAME
