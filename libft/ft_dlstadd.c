#include "libft.h"

void	ft_dlstadd(t_dlist **dlst, t_dlist *new)
{
	if (dlst != NULL && *dlst == NULL)
		*dlst = new;
	else if (dlst != NULL && *dlst != NULL && new != NULL)
	{
		new->next = *alst;
		(*alst)->prev = new;
		*alst = new;
	}
}
