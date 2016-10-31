#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
# include	<Windows.h>
#else
# include	<sys/mman.h>
# include	<unistd.h>
#endif
#include	<string.h>
#include	"gen_func_call.h"

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

void*	gen_func_call_win32(void* this, unsigned char* buff, void* func)
{
  const unsigned char	template[] = {
    0x83, 0xEC, 0x04,					// sub	esp, 4
    0x50,						// push	eax
    0x8B, 0x44, 0x24, 0x08,				// mov	eax, [esp+8]
    0x89, 0x44, 0x24, 0x04,				// mov	[esp+4], eax
    0xC7, 0x44, 0x24, 0x08, 0x00, 0x00, 0x00, 0x00,	// mov	[esp+8], 0
    0x58,						// pop	eax
    0xE9, 0x00, 0x00, 0x00, 0x00			// jmp	0
  };

  // Allow execution of the buffer
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
  MEMORY_BASIC_INFORMATION	mbi;

  VirtualQuery(buff, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
  // Exec protection is handled with 4 different flags (rx, rwx etc), we'll handle that in a generic way.
  if (!(mbi.Protect & 0xF0))
    {
      mbi.Protect |= (mbi.Protect & 0x0F) << 4; // Add execute flag
      mbi.Protect &= 0xFFFFFFF0; // Remove old protection flag
      VirtualProtect(buff, 26, mbi.Protect, &mbi.AllocationProtect); // We don't need the old protection, but VirtualProtect absolutely want to give it to us, so we'll store it in a var we don't use.
    }
#else
  // The only way to get page protection on unix seems to parse /proc/self/maps. Our structure needs rwx access rights anyway, so I won't bother with that.
  static size_t	page_size = 0;
  size_t	addr;

  if (page_size == 0)
    page_size = sysconf(_SC_PAGESIZE);
  // Align our buffer to the page size
  addr = (size_t)buff / page_size;
  addr *= page_size;
  mprotect((void*)addr, 1, PROT_READ | PROT_WRITE | PROT_EXEC); // 1 includes the entire page.
#endif

  memcpy(buff, template, 26);
  memcpy(buff + 16, &this, 4);
  int	diff = (unsigned int)func - ((unsigned int)buff + 26);
  memcpy(buff + 22, &diff, 4);
  return buff;
}
