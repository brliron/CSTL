#include	<stdlib.h>
#include	"util.h"

#ifndef LIST_H_
# define LIST_H_

struct	list_base;

struct				list_node
{
  struct list_base*		parent;
  struct list_node*		prev;
  struct list_node*		next;
  char				data[1];
};

struct			list_base
{
  struct list_node*	front;
  struct list_node*	back;
  int			size;

  int			node_size;
  int			data_size;

  cstl_iterator		it;
  /* I can't add new fields after this one because the size of the generic
  ** iterator may be different from the original one. */
};

void			list_generic_push_front(void* this_, void* val);
void			list_generic_push_back(void* this_, void* val);
void			list_generic_delete(struct list_base* this);

#endif /* !LIST_H_ */




#ifndef CSTL_DONT_DEFINE_TYPE

# if !defined(CSTL_TYPE) || !defined(CSTL_NAME)
#  error you muse define CSTL_TYPE and CSTL_NAME.
# endif

# define		MKNAME(begin, end)	TOKENPASTE2(TOKENPASTE2(begin ## list, CSTL_NAME), end)


MKNAME(struct _,);

/*************************
********** NODE **********
*************************/
typedef				MKNAME(struct _,_node)
{
  MKNAME(struct _,)*		parent;
  MKNAME(struct _,_node)*	prev;
  MKNAME(struct _,_node)*	next;
  CSTL_TYPE			data;
}				MKNAME(,_node);

/*************************
******** ITERATOR ********
*************************/
# pragma pack(push, 1)
typedef				MKNAME(struct _,_iterator)
{
  unsigned char			_next_jmp[CSTL_FUNC_CALL_SIZE];
  struct _cstl_iterator*	(*next)();
  int				data_size;
  CSTL_TYPE			data;
  MKNAME(,_node)*		node;
}				MKNAME(,_iterator);
# pragma pack(pop)

static cstl_iterator*	MKNAME(,_next)(MKNAME(,_iterator)* it)
{
  if (it->node->next == NULL)
    return NULL;
  it->node = it->node->next;
  it->data = it->node->data;
  return (cstl_iterator*)it;
}



/*************************
********** BASE **********
*************************/
typedef			MKNAME(struct _,)
{
  MKNAME(,_node)*	front;
  MKNAME(,_node)*	back;
  int			size;

  int			node_size;
  int			data_size;

  MKNAME(,_iterator)	it;

  unsigned char		_push_front_jmp[CSTL_FUNC_CALL_SIZE];
  void			(*push_front)(CSTL_TYPE val);
  unsigned char		_push_back_jmp[CSTL_FUNC_CALL_SIZE];
  void			(*push_back)(CSTL_TYPE val);
  unsigned char		_begin_jmp[CSTL_FUNC_CALL_SIZE];
  cstl_iterator*	(*begin)();
  unsigned char		_delete_jmp[CSTL_FUNC_CALL_SIZE];
  void			(*delete)();
}			MKNAME(,);




/*************************
******** FUNCTIONS *******
*************************/
static void	CSTL_FUNC_CALL_DEF(MKNAME(,_push_front), CSTL_TYPE val)
{
  CSTL_FUNC_CALL_PRE
  list_generic_push_front(this, &val);
}

static void	CSTL_FUNC_CALL_DEF(MKNAME(,_push_back), CSTL_TYPE val)
{
  CSTL_FUNC_CALL_PRE
  list_generic_push_back(this, &val);
}

static cstl_iterator*	CSTL_FUNC_CALL_DEF(MKNAME(,_begin))
{
  CSTL_FUNC_CALL_PRE
  if (this->front == NULL)
    return NULL;
  this->it.node = this->front;
  this->it.data = this->front->data;
  return (cstl_iterator*)&this->it;
}

CSTL_UNUSED static MKNAME(,)*	MKNAME(,_new)()
{
  MKNAME(,)*	this;

  this = malloc(sizeof(MKNAME(,)));
  this->front = NULL;
  this->back = NULL;
  this->size = 0;
  this->node_size = sizeof(MKNAME(,_node));
  this->data_size = sizeof(CSTL_TYPE);
  this->push_front	= gen_func_call(this, this->_push_front_jmp,	MKNAME(,_push_front));
  this->push_back	= gen_func_call(this, this->_push_back_jmp,	MKNAME(,_push_back));
  this->begin		= gen_func_call(this, this->_begin_jmp,		MKNAME(,_begin));
  this->delete		= gen_func_call(this, this->_delete_jmp,	list_generic_delete);

  this->it.data_size = sizeof(CSTL_TYPE);
  this->it.next	= gen_func_call(&this->it, this->it._next_jmp, MKNAME(,_next));
  return this;
}


# undef CSTL_TYPE
# undef CSTL_NAME
# undef MKNAME

#endif /* !CSTL_DONT_DEFINE_TYPE */
