#define CSTL_DONT_DEFINE_TYPE
#include	"list.h"

void			list_generic_push_front(void* this_, void* val)
{
  struct list_base*	this;
  struct list_node*	node;

  this = this_;
  node = malloc(this->node_size);
  memcpy(node->data, val, this->data_size);
  node->parent = this;
  node->prev = NULL;
  node->next = this->front;
  if (this->front)
    this->front->prev = node;
  this->front = node;
  if (this->back == NULL)
    this->back = node;
  this->size++;
}

void			list_generic_push_back(void* this_, void* val)
{
  struct list_base*	this;
  struct list_node*	node;

  this = this_;
  node = malloc(this->node_size);
  memcpy(node->data, val, this->data_size);
  node->parent = this;
  node->prev = this->back;
  node->next = NULL;
  if (this->back)
    this->back->next = node;
  if (this->front == NULL)
    this->front = node;
  this->back = node;
  this->size++;
}

void			list_generic_delete(struct list_base* this)
{
  struct list_node*	node;
  struct list_node*	next;

  node = this->front;
  while (node)
    {
      next = node->next;
      free(node);
      node = next;
    }
  free(this);
}
