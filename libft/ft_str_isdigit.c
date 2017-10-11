#include "libft.h"

int	ft_str_isdigit(char *str)
{
	int 	i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}
