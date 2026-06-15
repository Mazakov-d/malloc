#include "malloc.h"

int	main(void)
{
	void	*ptr_a;
	char	*ptr_b;
	char	*ptr_c;

	show_alloc_mem();
	ptr_a = malloc(0);
	ptr_b = malloc(10);
	// ft_printf_fd(1, "-------------\n\n");
	// show_alloc_mem();
	free(ptr_a);
	ptr_c = malloc(500);
	ft_strlcpy(ptr_c, "salut la vie c'est cool d'etre ici", 34);
	ft_printf_fd(1, "%s\n", ptr_c);
	ft_printf_fd(1, "-------------\n\n");
	show_alloc_mem();
	ptr_c = realloc(ptr_c, 10);
	ft_printf_fd(1, "%s\n", ptr_c);
	ft_printf_fd(1, "-------------\n\n");
	show_alloc_mem();
	free(ptr_c);
	ft_strlcpy(ptr_b, "salut", 6);
	ptr_b = realloc(ptr_b, 20);
	// ft_printf_fd(1, "\n\n-------------\n\n");
	// show_alloc_mem();
	ptr_b = realloc(ptr_b, 10000);
	ft_printf_fd(1, "\n\n------------\n\n");
	ft_printf_fd(1, "%s\n", ptr_b);
	ft_printf_fd(1, "\n\n------------\n\n");
	show_alloc_mem();
	free(ptr_b);
	ft_printf_fd(1, "\n\n-------------\n\n");
	show_alloc_mem();
	return (0);
}
