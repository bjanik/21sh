/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 16:40:23 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/08 19:52:54 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "bsh.h"

t_pipes	*init_pipes(void)
{
	t_pipes	*pipes;

	if (!(pipes = (t_pipes*)malloc(sizeof(t_pipes))))
		exit(EXIT_FAILURE);
	pipes->pipes_fd = NULL;
	pipes->nb_pipes = 0;
	return (pipes);
}

int		get_pipes_fd(t_exec *exec, t_pipes *pipes)
{
	int	i;

	while (exec && exec->cmd_separator == PIPE)
	{
		exec = exec->next;
		pipes->nb_pipes++;
	}
	i = -1;
	if (pipes->nb_pipes)
	{
		if (!(pipes->pipes_fd = (int**)malloc(sizeof(int*) * (pipes->nb_pipes))))
			exit(EXIT_FAILURE);
		while (++i < pipes->nb_pipes)
		{
			if (!(pipes->pipes_fd[i] = (int*)malloc(sizeof(int) * 2)))
				exit(EXIT_FAILURE);
		}
	}
	return (pipes->nb_pipes);
}

void	create_pipes(t_pipes *pipes)
{
	int	i;

	i = -1;
	while (++i < pipes->nb_pipes)
	{
		if (pipe(pipes->pipes_fd[i]) < 0)
			exit(EXIT_FAILURE);
	}
}

void	connect_processes_pipes(t_pipes *pipes, int i)
{
	if (!i)
	{
		if (dup2(pipes->pipes_fd[i][WRITE], STDOUT) < 0)
			exit(EXIT_FAILURE);
	}
	else if (i < pipes->nb_pipes)
	{
		if (dup2(pipes->pipes_fd[i - 1][READ], STDIN) < 0)
			exit(EXIT_FAILURE);
		if (dup2(pipes->pipes_fd[i][WRITE], STDOUT) < 0)
			exit(EXIT_FAILURE);
	 }
	 else
	 {
		if (dup2(pipes->pipes_fd[i - 1][READ], STDIN) < 0)
			exit(EXIT_FAILURE);
	}
}

void		close_pipes_fds(t_pipes *pipes)
{
	int	i;

	i = -1;
	while (++i < pipes->nb_pipes)
	{
		close(pipes->pipes_fd[i][READ]);
		close(pipes->pipes_fd[i][WRITE]);
	}
}
