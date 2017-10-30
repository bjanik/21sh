/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 16:04:13 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/24 17:46:30 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		clear_token_list(t_token **token_lst)
{
	t_token	*lst;

	lst = *token_lst;
	while (lst)
	{
		*token_lst = lst->next;
		ft_strdel(&lst->token);
		lst->next = NULL;
		ft_memdel((void**)lst);
		lst = *token_lst;
	}
}

static void	del(void *content, size_t size)
{
	(void)size;
	ft_memdel(&content);
}

void		clear_exec(t_exec **exec)
{
	t_exec	*ex;
	t_redir	*rd;
	t_list	*wd;

	if (!exec || !*exec)
		return ;
	ex = *exec;
	wd = ex->word_list;
	while (ex)
	{
		ft_lstdel(&ex->word_list, del);
		ex->last_word = NULL;
		rd = ex->redir_list;
		ex->prev = NULL;
		ex->last_redir = NULL;
		while (rd)
		{
			ex->redir_list = rd->next;
			ft_strdel(&rd->filename);
			ft_strdel(&rd->here_end);
			rd->next = NULL;
			ft_memdel((void**)&rd);
			rd = ex->redir_list;
		}
		ex = ex->next;
		ft_memdel((void**)exec);
		*exec = ex;
	}
}
