/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstdelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 13:16:51 by bjanik            #+#    #+#             */
/*   Updated: 2016/11/08 16:57:24 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstdelone(t_dlst **alst, void (*del)(void *, size_t))
{
	if (alst != NULL && *alst != NULL && del != NULL)
	{
		del((*alst)->data, (*alst)->data_size);
		if ((*alst)->prev != NULL)
			(*alst)->prev->next = NULL;
		free(*alst);
		*alst = NULL;
	}
}
