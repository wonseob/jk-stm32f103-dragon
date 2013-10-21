#ifndef QUEUE_H
#define QUEUE_H

#include "hw_config.h"
#include <stm32f10x.h>


typedef struct struct_q_node
{
	struct struct_q_node  *prev;
	struct struct_q_node  *next;
	s16	  len;
	void* data;
} q_node_type;

typedef struct struct_q_list
{
	struct struct_q_node *first;
	struct struct_q_node *last;
	struct struct_q_node *curr;
	s16 count;
	//int datasize;
	
} q_list_type;


extern void q_init_list(q_list_type* list);
extern void q_add_tail(q_list_type* list, q_node_type* node);
extern void q_add_head(q_list_type* list, q_node_type* node);
extern void q_remove_node(q_list_type* list, q_node_type* node);
extern q_node_type* q_remove_head(q_list_type* list);
extern q_node_type* q_remove_tail(q_list_type* list);
extern void q_remove_all(q_list_type* list);
s16 q_get_count(q_list_type* list);

#endif /* QUEUE_H */
