#include "malloc.h"
#include <unistd.h>

int main()
{
	char	*ptr = ft_malloc(10);

	ptr[0] = 'a';
	ptr[1] = 0;
	ft_printf_fd(1, "%s\n", ptr);
}