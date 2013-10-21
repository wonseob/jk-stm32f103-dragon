
#include "stm32f10x.h"
#include "queue.h"


/* ------------------------------------------------------------------------
** For Win32 Systems, users may want to enable mutual exclusion for the
** queue operations, this provides a mutex per queue along with locking
** and unlocking primitives.
** ------------------------------------------------------------------------ */



#define q_lock( q ) INTLOCK( )
#define q_free( q ) INTFREE( )


void q_init_list(q_list_type* list)
{
	list->first = list->last = NULL;
}

void q_add_tail(q_list_type* list, q_node_type* node)
{
	node->next = list->first;
	node->prev = NULL;

	if( list->first )
		list->first->prev = node;
	else
		list->last = node;

	list->first = node;
	list->count++;
}

void q_add_head(q_list_type* list, q_node_type* node)
{
	node->next = list->first;
	node->prev = NULL;

	if( list->first )
		list->first->prev = node;
	else
		list->last = node;

	list->first = node;
	list->count++;
}

void q_remove_node(q_list_type* list, q_node_type* node)
{
	if( node->next )
		node->next->prev = node->prev;
	else
		list->last = node->prev;

	if( node->prev )
		node->prev->next = node->next;
	else
		list->first = node->next;

	list->count--;
}

q_node_type* q_remove_head(q_list_type* list)
{
	q_node_type* node = list->first;

	if( !node )
		return NULL;

	q_remove_node(list, node);
	return node;
}

q_node_type* q_remove_tail(q_list_type* list)
{
	q_node_type* node = list->last;

	if( !node )
		return NULL;

	q_remove_node(list, node);
	return node;
}

void q_remove_all(q_list_type* list)
{
	q_node_type* node;
	while((node = q_remove_head(list)) != NULL );
}

s16 q_get_count(q_list_type* list)
{
	return list->count;
}
