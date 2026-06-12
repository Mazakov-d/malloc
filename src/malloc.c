#include "malloc.h"


void	*allocate_memory(size_t size)
{
	t_memory_chunk	*chunk;

	chunk = get_free_chunk(size);
	if (!chunk)
		return chunk;
	split_chunk(chunk, size);
	return chunk;
}

void *ft_malloc(size_t size)
{
	void	*ptr;
	t_ctx	*ctx;

	ctx = get_context();
	if (!ctx)
	{
		ctx = create_context();
		if (!ctx)
			return NULL;
	}
	ft_printf_fd(1, "%p\n", ctx);
	ptr = allocate_memory(align_up(size));
	return ptr;
}
