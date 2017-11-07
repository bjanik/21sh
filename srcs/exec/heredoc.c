/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:46:36 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/07 15:18:37 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int		write_to_heredoc_pipe(t_redir *redir)
{
	t_bsh	*bsh;

	bsh = get_bsh();
	while (42)
	{
		display_prompt(bsh->input);
		waiting_for_input(bsh->input);
		bsh->input->buffer[--bsh->input->buffer_len] = '\0';
		if (!ft_strcmp(bsh->input->buffer, redir->here_end))
			break ;
		bsh->input->buffer[bsh->input->buffer_len++] = '\n';
		write(redir->heredoc_pipe[WRITE], bsh->input->buffer,
				bsh->input->buffer_len);
	}
	return (0);
}

int		process_heredoc(t_exec *exec)
{
	t_redir	*rd;

	rd = exec->redir_list;
	while (rd)
	{
		if (rd->here_end)
		{
			pipe(rd->heredoc_pipe);
			write_to_heredoc_pipe(rd);
			exec->is_heredoc = 1;
		}
		rd = rd->next;
	}
	return (0);
}

int		redir_heredoc(t_redir *redir)
{
	if (dup2(redir->heredoc_pipe[READ], STDIN) < 0)
	close(redir->heredoc_pipe[WRITE]);
	close(redir->heredoc_pipe[READ]);
	return (0);
}

void	close_heredoc_pipes(t_redir *redir)
{
	t_redir	*rd;

	rd = redir;
	while (rd)
	{
		if (rd->type == DLESS)
		{
			close(rd->heredoc_pipe[WRITE]);
			close(rd->heredoc_pipe[READ]);
		}
		rd = rd->next;
	}
}
