/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:46:36 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/25 11:38:44 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static void	append_to_heredoc_data(t_redir *redir, t_list *here_in)
{
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

static int	save_heredoc(t_redir *redir)
{
	t_input	*input;
	t_list	*here_in;

	input = get_bsh()->input;
	while (42)
	{
		display_basic_prompt(input->term);
		if (wait_for_input(input, HEREDOC_INPUT) == STOP_HEREDOC)
			break ;
		input->buffer[--input->buffer_len] = '\0';
		if (input->buffer_len > 1)
		{
			if (!ft_strcmp(input->buffer, redir->here_end))
				break ;
		}
		input->buffer[input->buffer_len++] = '\n';
		here_in = ft_lstnew(input->buffer, input->buffer_len);
		append_to_heredoc_data(redir, here_in);
	}
	return (0);
}

void		handle_heredocs(t_exec *exec)
{
	t_exec	*ex;
	t_redir	*rd;

	ex = exec;
	while (ex)
	{
		rd = ex->redir_list;
		while (rd)
		{
			if (rd->here_end)
				save_heredoc(rd);
			rd = rd->next;
		}
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
