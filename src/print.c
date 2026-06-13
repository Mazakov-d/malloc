#include "malloc.h"

size_t	print_zone(t_page *page)
{
	t_page			*ptr;
	t_memory_chunk	*chunk;
	size_t			total_size;

	total_size = 0;
	ptr = page;
	while (ptr)
	{
		total_size += page->page_size;
		printf("Zone ptr: %p\n", ptr);
		printf("Page size: %zu\n", ptr->page_size);
		chunk = ptr->first_chunk;
		while (chunk)
		{
			printf("\nChunk ptr = %p\n", chunk);
			printf("Is free: %d\n", chunk->is_free);
			printf("Size: %zu\n", chunk->size);
			printf("Ptr to page: %p\n\n", chunk->page);
			chunk = chunk->next;
		}
		ptr = ptr->next;
	}
	return (total_size);
}

void	print_context(void)
{
	t_ctx	*g_ctx;
	size_t	total_size;

	total_size = 0;
	g_ctx = get_context();
	if (!g_ctx)
	{
		printf("Ctx not set.\n");
		return ;
	}
	printf("The struc g_ctx point to: %p\n", g_ctx);
	printf("The sizes of the struct (align up at 8):\n");
	printf("	t_ctx: %zu\n	t_memory: %zu\n	t_page: %zu\n", g_ctx->ctx_s, g_ctx->memory_chunk_s, g_ctx->page_s);
	printf("\nThe different zone point to: \n");
	printf("	tiny: %p\n	small: %p\n	large: %p\n	others: %p\n", g_ctx->tiny, g_ctx->small, g_ctx->large, g_ctx->others);
	printf("\n\n--TINY--\n");
	total_size += print_zone(g_ctx->tiny);
	printf("\n\n--SMALL--\n");
	total_size += print_zone(g_ctx->small);
	printf("\n\n--LARGE--\n");
	total_size = print_zone(g_ctx->large);
	printf("\n\n--OTHERS--\n");
	total_size += print_zone(g_ctx->others);
	printf("Total size: %zu\n\n\n", total_size);
}
