#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
# include	<Windows.h>
#else
# include	<sys/mman.h>
# include	<unistd.h>
#endif
#include	<string.h>
#include	"util.h"

/*
** With modular C, the language doesn't know what an object is, and so it doesn't pass this
** automatically to a member function. So, if we want to call v->push_back(5),
** we need to call v->push_back(v, 5). This is annoying, especially when the object on which
** the function is called is returned by a function, because you need to store it in
** a temporary variable.
**
** This project aims to have something nice to use, as close to C++ as possible.
** And I thought of a way to make a function call like v->push_back(5) work:
** push_back doesn't contain a pointer to the push_back function. Instead, it contains
** a pointer to a memory buffer, containing on the fly generated code. This code
** adds our object (hardcoded in the generated code) on the stack and jump to the push_back function.
*/

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
static void	memory_allow_execution(void* addr)
{
  MEMORY_BASIC_INFORMATION	mbi;

  VirtualQuery(addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
  // Exec protection is handled with 4 different flags (rx, rwx etc), we'll handle that in a generic way.
  if (!(mbi.Protect & 0xF0))
    {
      mbi.Protect |= (mbi.Protect & 0x0F) << 4; // Add execute flag
      mbi.Protect &= 0xFFFFFFF0; // Remove old protection flag
      VirtualProtect(addr, 26, mbi.Protect, &mbi.AllocationProtect); // We don't need the old protection, but VirtualProtect absolutely want to give it to us, so we'll store it in a var we don't use.
    }
}
#else /* WINDOWS */
static void	memory_allow_execution(void* addr)
{
  // The only way to get page protection on unix seems to parse /proc/self/maps. Our structure needs rwx access rights anyway, so I won't bother with that.
  static size_t	page_size = 0;
  size_t	int_addr;

  if (page_size == 0)
    page_size = sysconf(_SC_PAGESIZE);
  // Align our buffer to the page size
  int_addr = (size_t)addr / page_size;
  int_addr *= page_size;
  mprotect((void*)int_addr, 1, PROT_READ | PROT_WRITE | PROT_EXEC); // 1 includes the entire page.
}
#endif /* WINDOWS */

#ifdef __i386__
void*	gen_func_call(void* this, unsigned char* buff, void* func)
{
  const unsigned char	template[] = {
    0x83, 0xEC, 0x08,					// sub	esp, 8		; Add enough space on the stack for our 2 variables
    0xC7, 0x44, 0x24, 0x04, 0x00, 0x00, 0x00, 0x00,	// mov	[esp+4], 0	; Put the object on the stack
    0xC7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00,		// mov	[esp], 0	; Put the address of our cleanup code on the stack
    0xE9, 0x00, 0x00, 0x00, 0x00,			// jmp	0		; Jump to our function // TODO: replace with a call
    // After the function call
    0x83, 0xC4, 0x04,					// add	esp, 4		; Our return value have been poped. Pop the object.
    0xC3						// ret
  };

  memory_allow_execution(buff);
  memcpy(buff, template, CSTL_FUNC_CALL_SIZE);

  memcpy(buff + 7, &this, 4); // Put our object on the stack
  void*	ret_addr = buff + 23;
  memcpy(buff + 14, &ret_addr, 4); // Put the address of our cleanup code on the stack
  int	diff = (unsigned int)func - ((unsigned int)buff + 23);
  memcpy(buff + 19, &diff, 4);
  return buff;
}
#elif defined(__x86_64__)
# if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
void*	gen_func_call(void* this, unsigned char* buff, void* func)
{
  const unsigned char	template[] = {
    0x4d, 0x89, 0xc1,						// mov	r9,	r8
    0x49, 0x89, 0xd0,						// mov	r8,	rdx
    0x48, 0x89, 0xca,						// mov	rdx,	rcx
    0x48, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// mov	rcx,	0
    0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// mov	rax,	0
    0xff, 0xe0							// jmp	rax
  };

  memory_allow_execution(buff);
  memcpy(buff, template, CSTL_FUNC_CALL_SIZE);

  memcpy(buff + 11, &this, 8);
  memcpy(buff + 21, &func, 8);
  return buff;
}
# else /* WINDOWS */
void*	gen_func_call(void* this, unsigned char* buff, void* func)
{
  const unsigned char	template[] = {
    0x4d, 0x89, 0xc1,						// mov	r9,	r8
    0x49, 0x89, 0xc8,						// mov	r8,	rcx
    0x48, 0x89, 0xd1,						// mov	rcx,	rdx
    0x48, 0x89, 0xf2,						// mov	rdx,	rsi
    0x48, 0x89, 0xfe,						// mov	rsi,	rdi
    0x48, 0xbf, 0x9a, 0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00,	// mov	rdi,	0
    0x48, 0xb8, 0x9a, 0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00,	// mov	rax,	0
    0xff, 0xe0							// jmp	rax
  };

  memory_allow_execution(buff);
  memcpy(buff, template, CSTL_FUNC_CALL_SIZE);

  memcpy(buff + 17, &this, 8);
  memcpy(buff + 27, &func, 8);
  return buff;
}
# endif /* WINDOWS */
#endif /* i386 or x86_64 */
