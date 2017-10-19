/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 13:25:39 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/17 15:44:10 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*lst;

	if (alst == NULL || del == NULL || *alst == NULL)
		return ;
	lst = *alst;
	while (*alst)
	{
		*alst = (*alst)->next;
		del(lst->content, lst->content_size);
		ft_memdel((void**)&lst);
		lst = *alst;
	}
}
