#ifndef __LIBCON_MACROS_H__
#define __LIBCON_MACROS_H__

#ifndef HAVE_OFFSETOF
#define HAVE_OFFSETOF
#define offsetof(type_t, member) (&((type_t*)0)->member)
#endif

#define HAVE_COTANOF
#define cotanof(ptr, type_t, member) (type_t)((char*)ptr - (int)offsetof(type_t, member))

#define HAVE_COUNTOF
#define countof(array) (sizeof(array) / sizeof(array[0]))

#endif /* __LIBCON_MACROS_H__ */