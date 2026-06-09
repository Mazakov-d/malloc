#include "malloc.h"

void	*ft_realloc(void *ptr, size_t size)
{
	t_memory_chunk	*chunk;
	void			*new_ptr;

	if (!ptr)
		return ft_malloc(size);
	chunk = ptr - sizeof(t_memory_chunk);
	if (chunk->size - get_chunk_size(sizeof(t_memory_chunk)) >= size)
		return ptr;
	new_ptr = ft_malloc(size);
	if (!new_ptr)
	{
		errno = EAGAIN;
		return (ptr);
	}
	return NULL;
}