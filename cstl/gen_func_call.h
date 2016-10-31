#ifndef CSTL_GEN_FUNC_CALL_H_
# define CSTL_GEN_FUNC_CALL_H_

# define CSTL_FUNC_CALL_SIZE	27

void*		gen_func_call_win32(void* this, unsigned char* buff, void* func);
# define	gen_func_call	gen_func_call_win32

#endif /* !CSTL_GEN_FUNC_CALL_H_ */
