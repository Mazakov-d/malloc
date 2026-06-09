#include "malloc.h"
#include <unistd.h>

int main()
{
	int ret = getpagesize();
	// char	*ptr = ft_malloc(10);

	// ptr[0] = 'a';
	// ptr[1] = 0;
	ft_printf_fd(1, "%i\n", ret);
}