#include "malloc.h"

size_t	print_zone(t_page *page)
{
	t_page			*ptr;
	t_memory_chunk	*chunk;
	size_t			total_size;

	total_size = 0;
	ptr = page;
	if (!ptr)
		ft_printf_fd(1, "Page not set.\n\n");
	while (ptr)
	{
		total_size += ptr->page_size;
		ft_printf_fd(1, "Page size: %d\n", ptr->page_size);
		chunk = ptr->first_chunk;
		while (chunk)
		{
			ft_printf_fd(1, "\nChunk\n");
			ft_printf_fd(1, "Is free: %d\n", chunk->is_free);
			ft_printf_fd(1, "Size: %d\n", chunk->size);
			chunk = chunk->next;
		}
		ptr = ptr->next;
	}
	return (total_size);
}

void	show_alloc_mem(void)
{
	t_ctx	*g_ctx;
	size_t	total_size;

	total_size = 0;
	g_ctx = get_context();
	if (!g_ctx)
	{
		ft_printf_fd(1, "Ctx not set.\n");
		return ;
	}
	ft_printf_fd(1, "The struc g_ctx is set.\n");
	ft_printf_fd(1, "The sizes of the struct (align up at 8):\n");
	ft_printf_fd(1, "	t_ctx: %d\n	t_memory: %d\n	t_page: %d\n", g_ctx->ctx_s, g_ctx->memory_chunk_s, g_ctx->page_s);
	ft_printf_fd(1, "\n\n--TINY--\n");
	total_size += print_zone(g_ctx->tiny);
	ft_printf_fd(1, "\n\n--SMALL--\n");
	total_size += print_zone(g_ctx->small);
	ft_printf_fd(1, "\n\n--LARGE--\n");
	total_size += print_zone(g_ctx->large);
	ft_printf_fd(1, "\n\n--OTHERS--\n");
	total_size += print_zone(g_ctx->others);
	ft_printf_fd(1, "\n\nTotal size: %d\n\n\n", total_size);
}
