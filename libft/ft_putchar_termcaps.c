#include "libft.h"


int	ft_putchar_termcaps(int c)
{
	return (write(STDIN, &c, 1));
}
