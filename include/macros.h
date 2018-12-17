#ifndef __LIBCON_MACROS_H__
#define __LIBCON_MACROS_H__

#define countof(array) (sizeof(array) / sizeof(array[0]))

#ifndef HAVE_OFFSETOF
#define offsetof(type_t, member) (&((type_t*)0)->member)
#endif

#define HAVE_COTANOF
#define cotanof(ptr, type_t, member) (type_t)((char*)ptr - offsetof(type_t, member))

#endif /* __LIBCON_MACROS_H__ */