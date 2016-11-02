#include	<stdlib.h>
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

void	vector_generic_push_back(void* this_, void* val);
void	vector_generic_delete(void* this);

#endif /* !VECTOR_H_ */




#ifndef CSTL_DONT_DEFINE_TYPE

# if !defined(CSTL_TYPE) || !defined(CSTL_NAME)
#  error you muse define CSTL_TYPE and CSTL_NAME.
# endif

# define		MKNAME(begin, end)	TOKENPASTE2(TOKENPASTE2(begin ## vector, CSTL_NAME), end)


MKNAME(struct _,);

// ==========================
// ======== ITERATOR ========
// ==========================
# pragma pack(push, 1)
typedef				MKNAME(struct _,_iterator)
{
  unsigned char			_next_jmp[CSTL_FUNC_CALL_SIZE];
  struct _cstl_iterator*	(*next)();
  int				data_size;
  CSTL_TYPE			data;
  MKNAME(struct _,)*		parent;
  int				i;
}				MKNAME(,_iterator);
# pragma pack(pop)



// ==========================
// ========== BASE ==========
// ==========================
typedef		MKNAME(struct _,)
{
  CSTL_TYPE*	data;
  int		size;
  int		data_size;

  MKNAME(,_iterator)	it;

  unsigned char		_push_back_jmp[CSTL_FUNC_CALL_SIZE];
  void			(*push_back)(CSTL_TYPE val);
  unsigned char		_begin_jmp[CSTL_FUNC_CALL_SIZE];
  cstl_iterator*	(*begin)();
  unsigned char		_delete_jmp[CSTL_FUNC_CALL_SIZE];
  void			(*delete)();
}			MKNAME(,);




// ==========================
// ======== FUNCTIONS =======
// ==========================
static cstl_iterator*	MKNAME(,_next)(MKNAME(,_iterator)* it)
{
  it->i++;
  if (it->i == it->parent->size)
    return NULL;
  it->data = it->parent->data[it->i];
  return (cstl_iterator*)it;
}

static cstl_iterator*	CSTL_FUNC_CALL_DEF(MKNAME(,_begin))
{
  CSTL_FUNC_CALL_PRE
  if (this->size == 0)
    return NULL;
  this->it.data = this->data[0];
  this->it.i = 0;
  return (cstl_iterator*)&this->it;
}

static void	CSTL_FUNC_CALL_DEF(MKNAME(,_push_back), CSTL_TYPE val)
{
  CSTL_FUNC_CALL_PRE
  vector_generic_push_back(this, &val);
}

__attribute__((unused)) static MKNAME(,)*	MKNAME(,_new)()
{
  MKNAME(,)*	this;

  this = malloc(sizeof(MKNAME(,)));
  this->data = NULL;
  this->size = 0;
  this->data_size = sizeof(CSTL_TYPE);
  this->push_back	= gen_func_call(this, this->_push_back_jmp,	MKNAME(,_push_back));
  this->begin		= gen_func_call(this, this->_begin_jmp,		MKNAME(,_begin));
  this->delete		= gen_func_call(this, this->_delete_jmp,	vector_generic_delete);

  this->it.data_size = sizeof(CSTL_TYPE);
  this->it.next	= gen_func_call(&this->it, this->it._next_jmp, MKNAME(,_next));
  this->it.parent = this;
  return this;
}


# undef CSTL_TYPE
# undef CSTL_NAME
# undef MKNAME

#endif /* !CSTL_DONT_DEFINE_TYPE */
