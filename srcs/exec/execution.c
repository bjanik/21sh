/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 15:50:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/02 15:06:48 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		run_builtin(int builtin, char **cmd)
{
	t_bsh	*bsh;
	bsh = get_bsh();
	
	if (handle_redirection(bsh->exec))
	{
		bsh->exit_status = 1;
		return ;
	}
	bsh->exec->exit_status = g_builtins[builtin].builtin(&(bsh->env), cmd);
}

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

static void	connect_pipe(int **pipes_fd, int nb_pipes, int i)
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

void		pipe_sequence(t_exec **exec, int **pipes_fd, int nb_pipes)
{
	int		i;
	int		pid[100];
	int		k;
	t_bsh	*bsh;
	t_exec	*ex;
	int		saved_fd[10];

	bsh = get_bsh();
	k = 0;
	i = -1;
	create_pipes(pipes_fd, nb_pipes);
	ex = *exec;
	while (++i <= nb_pipes)
	{
		(*exec)->cmd = lst_to_tab((*exec)->word_list, (*exec)->word_count);
		expand_words(bsh->exp, (*exec)->cmd);
		if (((*exec)->is_builtin = cmd_is_builtin((*exec)->cmd)) > -1)
			;
		else
		{
			if ((pid[k] = fork()) < 0)
				exit(EXIT_FAILURE);
			if (!pid[k++])
			{
				restore_initial_attr(bsh->term);
				connect_pipe(pipes_fd, nb_pipes, i);
				close_pipes_fds(pipes_fd, nb_pipes);
				run_binary(*exec, bsh->env);
			}
		}
		*exec = (*exec)->next;
	}
	i = -1;
	saved_fd[0] = dup(STDIN);
	saved_fd[1] = dup(STDOUT);
	saved_fd[2] = dup(STDERR);
	while (++i <= nb_pipes)
	{
		if (ex->is_builtin > -1)
		{
			connect_pipe(pipes_fd, nb_pipes, i);
			run_builtin(ex->is_builtin, ex->cmd);
		}
		ex = ex->next;
	}
	dup2(saved_fd[0], STDIN);
	dup2(saved_fd[1], STDOUT);
	dup2(saved_fd[2], STDERR);
	close_pipes_fds(pipes_fd, nb_pipes);
	while (k)
		waitpid(pid[--k], &bsh->exit_status, 0);
	restore_custom_attr(bsh->term);
}
