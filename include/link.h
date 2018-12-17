#ifndef __LIBCON_LINK_H__
#define __LIBCON_LINK_H__

typedef struct link_s
{
    struct link_s* prev;
    struct link_s* next;
} link_t;

/**
 * Attach lnk to tgt->next
 */
#define link_next(lnk, tgt)             \
    do {                                \
		(lnk)->next = (tgt)->next;      \
		(lnk)->prev = (tgt);            \
		(tgt)->next = (lnk);            \
    } while (0)

/**
 * Attach lnk to tgt->prev
 */
#define link_prev(lnk, tgt)             \
    do {                                \
		(lnk)->prev = (tgt)->prev;      \
		(lnk)->next = (tgt);            \
		(tgt)->prev = (lnk);            \
    } while (0)

/**
 * Detach the link from the chains
 */
#define link_null(lnk)                                      \
    do {                                                    \
		if ((lnk)->prev) (lnk)->prev->next = (lnk)->next;	\
		if ((lnk)->next) (lnk)->next->prev = (lnk)->prev;	\
		(lnk)->prev = NULL;                                 \
		(lnk)->next = NULL;                                 \
    } while (0)

/**
 * Get container of link
 */
#define link_data(lnk, type_t, member) ((lnk) ? ((type_t*)((char*)(lnk) - (int)&((type_t*)0)->member)) : NULL)

#endif /* __LIBCON_LINK_H__ */