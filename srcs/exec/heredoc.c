/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:46:36 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/12 18:06:33 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int	save_heredoc(t_redir *redir)
{
	t_input	*input;
	t_list	*here_in;

	input = get_bsh()->input;
	while (42)
	{
		display_prompt(input);
		waiting_for_input(input, HEREDOC_INPUT);
		if (input->buffer_len > 1)
			if (!ft_strncmp(input->buffer, redir->here_end,
						input->buffer_len - 1))
			break ;
		here_in = ft_lstnew(input->buffer, input->buffer_len);
		if (redir->heredoc_input[0] == NULL)
		{
			redir->heredoc_input[0] = here_in;
			redir->heredoc_input[1] = here_in;
		}
		else
		{
			redir->heredoc_input[1]->next = here_in;
			redir->heredoc_input[1] = here_in;
		}
	}
	return (0);
}

static void	save_heredoc_input(t_exec *exec)
{
	t_redir	*rd;

	rd = exec->redir_list;
	while (rd)
	{
		if (rd->here_end)
			save_heredoc(rd);
		rd = rd->next;
	}
}

void		handle_heredocs(t_exec *exec)
{
	t_exec	*ex;

	ex = exec;
	while (ex)
	{
		save_heredoc_input(ex);
		ex = ex->next;
	}
}

int			redir_heredoc(t_redir *redir)
{
	t_list	*here_data;

	here_data = redir->heredoc_input[0];
	if (pipe(redir->heredoc_pipe))
		exit(EXIT_FAILURE);
	while (here_data)
	{
		write(redir->heredoc_pipe[WRITE], here_data->content,
			here_data->content_size);
		here_data = here_data->next;
	}
	if (dup2(redir->heredoc_pipe[READ], STDIN) < 0)
		exit(EXIT_FAILURE);
	close(redir->heredoc_pipe[WRITE]);
	close(redir->heredoc_pipe[READ]);
	return (0);
}

void		close_heredoc_pipes(t_redir *redir)
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
