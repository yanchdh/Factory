#ifndef	_SLIST_H_
#define	_SLIST_H_

/*
 * Singly-linked List definitions.
 */
#define _SLIST_HEAD(name, type)						\
struct name {								\
	struct type *slh_first;	/* first element */			\
}

#define	_SLIST_HEAD_INITIALIZER(head)					\
	{ NULL }

#define _SLIST_ENTRY(type)						\
struct {								\
	struct type *sle_next;	/* next element */			\
}

/*
 * Singly-linked List access methods.
 */
#define	_SLIST_FIRST(head)	((head)->slh_first)
#define	_SLIST_END(head)		NULL
#define	_SLIST_EMPTY(head)	(_SLIST_FIRST(head) == _SLIST_END(head))
#define	_SLIST_NEXT(elm, field)	((elm)->field.sle_next)

#define	_SLIST_FOREACH(var, head, field)					\
	for((var) = _SLIST_FIRST(head);					\
	    (var) != _SLIST_END(head);					\
	    (var) = _SLIST_NEXT(var, field))

/*
 * Singly-linked List functions.
 */
#define	_SLIST_INIT(head) {						\
	_SLIST_FIRST(head) = _SLIST_END(head);				\
}

#define	_SLIST_INSERT_AFTER(slistelm, elm, field) do {			\
	(elm)->field.sle_next = (slistelm)->field.sle_next;		\
	(slistelm)->field.sle_next = (elm);				\
} while (0)

#define	_SLIST_INSERT_HEAD(head, elm, field) do {			\
	(elm)->field.sle_next = (head)->slh_first;			\
	(head)->slh_first = (elm);					\
} while (0)

#define	_SLIST_REMOVE_HEAD(head, field) do {				\
	(head)->slh_first = (head)->slh_first->field.sle_next;		\
} while (0)

#endif	/* !_SLIST_H_ */
