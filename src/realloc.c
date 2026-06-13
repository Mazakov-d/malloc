#include "malloc.h"

void	*ft_realloc(void *ptr, size_t size)
{
	t_memory_chunk	*header;
	t_ctx			*g_ctx;
	void			*new_ptr;
	size_t			i;

	g_ctx = get_context();
	header = (t_memory_chunk *)((char *)ptr - g_ctx->memory_chunk_s);
	if (header->size >= size)
		return (ptr);
	new_ptr = malloc(size);
	if (!new_ptr)
	{
		free(ptr);
		return (new_ptr);
	}
	i = 0;
	while (i < header->size)
	{
		((unsigned char *)new_ptr)[i] = ((unsigned char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
