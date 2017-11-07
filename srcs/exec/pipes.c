/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 15:49:55 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/06 10:17:57 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "bsh.h"

int		**get_pipes_fd(t_exec *exec, int *nb_pipes)
{
	int	**pipes_fd;
	int	i;

	pipes_fd = NULL;
	*nb_pipes = 0;
	while (exec && exec->cmd_separator == PIPE)
	{
		exec = exec->next;
		(*nb_pipes)++;
	}
	i = -1;
	if (*nb_pipes)
	{
		if (!(pipes_fd = (int**)malloc(sizeof(int*) * (*nb_pipes))))
			exit(EXIT_FAILURE);
		while (++i < (*nb_pipes))
		{
			if (!(pipes_fd[i] = (int*)malloc(sizeof(int) * 2)))
				exit(EXIT_FAILURE);
		}
	}
	return (pipes_fd);
}

void	create_pipes(int **pipes_fd, int nb_pipes)
{
	int	i;

	i = -1;
	while (++i < nb_pipes)
	{
		if (pipe(pipes_fd[i]) < 0)
			exit(EXIT_FAILURE);
	}
}

void    connect_processes_pipes(int **pipes_fd, int nb_pipes, int i)
{
        if (!i)
        {
                if (dup2(pipes_fd[i][WRITE], STDOUT) < 0)
                        exit(EXIT_FAILURE);
        }
        else if (i < nb_pipes)
        {
                if (dup2(pipes_fd[i - 1][READ], STDIN) < 0)
                        exit(EXIT_FAILURE);
                if (dup2(pipes_fd[i][WRITE], STDOUT) < 0)
                        exit(EXIT_FAILURE);
        }
        else
        {
                if (dup2(pipes_fd[i - 1][READ], STDIN) < 0)
                        exit(EXIT_FAILURE);
        }
}

void		close_pipes_fds(int **pipes_fd, int nb_pipes)
{
	int	i;

	i = -1;
	while (++i < nb_pipes)
	{
		close(pipes_fd[i][READ]);
		close(pipes_fd[i][WRITE]);
	}
}
