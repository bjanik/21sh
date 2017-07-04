#include "libft.h"

t_dlist	*ft_dlstnew(void const *data)
{
	t_dlist	*new;

	if ((new = (t_dlist*)malloc(sizeof(t_dlist)))== NULL)
		return (NULL);
	new->data = data;
	new->next = NULL;
	new->prev = NULL;
}
