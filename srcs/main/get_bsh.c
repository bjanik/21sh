/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bsh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:35:23 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/27 15:12:54 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static t_bsh		*init_bsh(void)
{
	t_bsh			*bsh;

	if (!(bsh = (t_bsh*)malloc(sizeof(t_bsh))))
		exit(EXIT_FAILURE);
	bsh->term = init_term();
	bsh->history = init_history();
	bsh->input = init_input(bsh->term, bsh->history);
	bsh->lexer = NULL;
	bsh->parser = NULL;
	bsh->exec = NULL;
	bsh->tokens[0] = NULL;
	bsh->tokens[1] = NULL;
	bsh->pid = getpid();
	bsh->exit_status = 0;
	bsh->shell_name = "-bsh";
	return (bsh);
}

t_bsh				*get_bsh(void)
{
	static t_bsh	*bsh = NULL;

	if (!bsh)
		bsh = init_bsh();
	return (bsh);
}
