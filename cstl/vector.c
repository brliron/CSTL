#define CSTL_DONT_DEFINE_TYPE
#include	<stdlib.h>
#include	"vector.h"

void			vector_generic_push_back(void* this_, void* val)
{
  struct vector_base*	this;

  this = this_;
  if (this->size % 10 == 0)
    this->data = realloc(this->data, (this->size + 10) * this->data_size);
  memcpy(this->data + this->size * this->data_size, val, this->data_size);
  this->size++;
}

void	vector_generic_delete(void* this)
{
  free(this);
}
