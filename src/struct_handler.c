#include "malloc.h"

t_page	*last_page()
{
	t_page	*ptr;
	t_ctx	*ctx;

	ctx = get_context();
	ptr = ctx->first_page;
	while (ptr->next)
		ptr = ptr->next;
	return ptr;
}

t_memory_chunk	*first_memory_chunk(t_memory_chunk *chunk)
{
	t_memory_chunk	*ptr;

	ptr = chunk;
	while (ptr->prev)
		ptr = ptr->prev;
	return ptr;
}

t_memory_chunk	*last_memory_chunk(t_memory_chunk *chunk)
{
	t_memory_chunk	*ptr;

	ptr = chunk;
	while (ptr->next)
		ptr = ptr->next;
	return ptr;
}

void	remove_node(t_memory_chunk *ptr)
{
	t_memory_chunk	*prev;
	t_memory_chunk	*next;

	if (!ptr)
		return;
	prev = ptr->prev;
	next = ptr->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	ptr->next = NULL;
	ptr->prev = NULL;
}