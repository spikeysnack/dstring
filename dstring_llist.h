/* dstring_llist.h */


/* LICENSE:   Freeware */
/* AUTHOR:    Unknown  */
/* COPYRIGHT: NONE     */



/* 
 * a double linked list implementation
 * based on list.h from  linux kernel
 * and e2fsprogs. This is generic and 
 * can be renamed and reused to make a list
 * of any struct.
 */


#ifndef DSTRING_LLIST_H
#define DSTRING_LLIST_H


#ifdef __GNUC__
#define _INLINE_ static __inline__
#else                         /* For Watcom C */
#define _INLINE_ static inline
#endif

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
_INLINE_ void __list_add(struct list_head * add,
	struct list_head * prev,
	struct list_head * next)
{
	next->prev = add;
	add->next = next;
	add->prev = prev;
	prev->next = add;
}

/**
 * list_add - add a new entry
 * @add:	new entry to be added
 * @head:	list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
_INLINE_ void list_add(struct list_head *add, struct list_head *head)
{
	__list_add(add, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @add:	new entry to be added
 * @head:	list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
_INLINE_ void list_add_tail(struct list_head *add, struct list_head *head)
{
	__list_add(add, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
_INLINE_ void __list_del(struct list_head * prev,
				  struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry:	the element to delete from the list.
 *
 * list_empty() on @entry does not return true after this, @entry is
 * in an undefined state.
 */
_INLINE_ void list_del(struct list_head *entry)
{
  	__list_del(entry->prev, entry->next);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry:	the element to delete from the list.
 */
_INLINE_ void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * list_empty - tests whether a list is empty
 * @head:	the list to test.
 */
_INLINE_ int list_empty(struct list_head *head)
{
	return head->next == head;
}

/**
 * list_last_entry - tests whether is entry last in the list
 * @entry:	the entry to test.
 * @head:	the list to test.
 */
_INLINE_ int list_last_entry(struct list_head *entry, struct list_head *head)
{
	return head->prev == entry;
}


/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
_INLINE_ void list_replace(struct list_head *old, struct list_head *new)
{
  new->next = old->next;
  new->next->prev = new;
  new->prev = old->prev;
  new->prev->next = new;
}


/**
 * list_splice - join two lists
 * @list:	the new list to add.
 * @head:	the place to add it in the first list.
 * @next:	the next header
 */

_INLINE_ void __list_splice(const struct list_head *list,
                                 struct list_head *prev,
                                 struct list_head *next)
{
  struct list_head *first = list->next;
  struct list_head *last = list->prev;

  first->prev = prev;
  prev->next = first;

  last->next = next;
  next->prev = last;
}


/**
 * list_splice - join two lists
 * @list:	the new list to add.
 * @head:	the place to add it in the first list.
 */
_INLINE_ void list_splice(struct list_head *list, struct list_head *head)
{
  if (!list_empty(list))
    __list_splice(list, head->prev, head);

  /*
	struct list_head *first = list->next;

	if (first != list) {
		struct list_head *last = list->prev;
		struct list_head *at = head->next;

		first->prev = head;
		head->next = first;

		last->next = at;
		at->prev = last;
	}
	*/
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
_INLINE_ void list_splice_tail(struct list_head *list, struct list_head *head)
{
  if (!list_empty(list))
    __list_splice(list, head->prev, head);
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
_INLINE_ void list_splice_tail_init(struct list_head *list,
                                         struct list_head *head)
{
  if (!list_empty(list)) {
    __list_splice(list, head->prev, head);
    INIT_LIST_HEAD(list);
  }
}



/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
  ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each - iterate over elements in a list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_backwardly - iterate over elements in a list in reverse
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each_backwardly(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_safe - iterate over elements in a list, but don't dereference
 *                      pos after the body is done (in case it is freed)
 * @pos:	the &struct list_head to use as a loop counter.
 * @pnext:	the &struct list_head to use as a pointer to the next item.
 * @head:	the head for your list (not included in iteration).
 */
#define list_for_each_safe(pos, pnext, head) \
	for (pos = (head)->next, pnext = pos->next; pos != (head); \
	     pos = pnext, pnext = pos->next)


/**
 * list_for_each_entry-iterate over list of given type
 * @pos:the type * to use as a loop counter.
 * @head:the head for your list.
 * @member:the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)\
  for (pos = list_entry((head)->next, typeof(*pos), member);\
  &pos->member != (head);\
       pos = list_entry(pos->member.next, typeof(*pos), member))



/**
 * Casts a member of a structure out to the containing structure
 * @param ptr        the pointer to the member.
 * @param type       the type of the container struct this is embedded in.
 * @param member     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
  const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
  (type *)( (char *)__mptr - offsetof(type,member) );})



#undef _INLINE_

#endif /* DSTRING_LLIST_H */
