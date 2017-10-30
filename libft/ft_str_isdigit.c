#include "libft.h"

int	ft_str_isdigit(char *str)
{
	int 	i;

	i = 0;
	if (str[0] != '+' && str[0] != '-' && !ft_isdigit(str[0]))
		return (0);
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}
