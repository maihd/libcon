#pragma once

#ifndef HAVE_OFFSETOF
#define HAVE_OFFSETOF
#define offsetof(type_t, member) (&((type_t*)0)->member)
#endif

#define HAVE_CONTAINER_OF
#define container_of(ptr, type_t, member) (type_t)((char*)ptr - (int)offsetof(type_t, member))

#define HAVE_COUNTOF
#define countof(array) (sizeof(array) / sizeof(array[0]))
