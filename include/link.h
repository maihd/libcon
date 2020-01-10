#pragma once

typedef struct Link
{
    struct Link* prev;
    struct Link* next;
} Link;

/**
 * Attach link to target->next
 */
#define Link_attachNext(link, target)       \
    do {                                	\
		(link)->next = (target)->next;      \
		(link)->prev = (target);            \
		(target)->next = (link);            \
    } while (0)

/**
 * Attach link to target->prev
 */
#define Link_attachPrev(link, target)       \
    do {                                	\
		(link)->prev = (target)->prev;      \
		(link)->next = (target);            \
		(target)->prev = (link);            \
    } while (0)

/**
 * Attach link
 */
#define Link_attach(link, PREV, NEXT)       \
    do {                                	\
		(link)->prev = PREV;      			\
		(link)->next = NEXT;            	\
    } while (0)

/**
 * Detach the link from the chains
 */
#define Link_detach(link)                                    	\
    do {                                                    	\
		if ((link)->prev) (link)->prev->next = (link)->next;	\
		if ((link)->next) (link)->next->prev = (link)->prev;	\
		(link)->prev = NULL;                                 	\
		(link)->next = NULL;                                 	\
    } while (0)

/**
 * Get container of link
 */
#define Link_container(link, T, member) ((link) ? ((T*)((char*)(link) - (int)&((T*)0)->member)) : NULL)
