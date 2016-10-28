#include	<stdio.h>
#define	CSTL_TYPE	int
#define	CSTL_NAME	int
#include	"cstl/vector.h"
#define	CSTL_TYPE	char*
#define	CSTL_NAME	str
#include	"cstl/vector.h"
#define	CSTL_TYPE	int
#define	CSTL_NAME	int
#include	"cstl/list.h"


int	main()
{
  vectorint*	v = vectorint_new();
  v->push_back(3);
  v->push_back(5);
  for_each(int, it, v,
	   {
	     int	var;
	     var = it + 1;
	     printf("%d + 1 = %d\n", it, var);
	   }
	   );
  v->delete();


  vectorstr*	v2 = vectorstr_new();
  v2->push_back("Test 1");
  v2->push_back("2ème test");
  for_each(char*, it, v2, printf("Str: %s\n", it));
  v2->delete();


  listint*	l = listint_new();
  l->push_back(3);
  l->push_back(5);
  for_each(int, n, l, printf("%d\n", n));
  l->delete();

  return 0;
}
