/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 15:50:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/28 18:34:39 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		run_builtin(int builtin, char **cmd)
{
	t_bsh	*bsh;
	int		save_stdout;
	int		save_stdin;
	int		save_stderr;

	bsh = get_bsh();
	save_stdout = dup(STDOUT);
	save_stderr = dup(STDERR);
	save_stdin = dup(STDIN);
	if (handle_redirection(bsh->exec))
	{
		bsh->exit_status = 1;
		return ;
	}
	bsh->exec->exit_status = g_builtins[builtin].builtin(&(bsh->env), cmd);
	dup2(save_stdout, STDOUT);
	dup2(save_stderr, STDERR);
	dup2(save_stdin, STDIN);
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
		if (!(pipes_fd = (int**)malloc(sizeof(int*) * *nb_pipes)))
			exit(EXIT_FAILURE);
		while (++i < (*nb_pipes))
			if (!(pipes_fd[i] = (int*)malloc(sizeof(int) * 2)))
				exit(EXIT_FAILURE);
	}
	return (pipes_fd);
}

static void	connect_pipe(int **pipes_fd, int nb_pipes, int i)
{
	if (!i)
	{
		if (dup2(pipes_fd[i][WRITE], STDOUT))
			exit(EXIT_FAILURE);
	}
	else if (i != nb_pipes)
	{
		if (dup2(pipes_fd[i][READ], STDIN))
			exit(EXIT_FAILURE);
		if (dup2(pipes_fd[i][WRITE], STDOUT))
			exit(EXIT_FAILURE);
	}
	if (i == nb_pipes)
		if (dup2(pipes_fd[i][READ], STDOUT))
			exit(EXIT_FAILURE);
	close(pipes_fd[i][WRITE]);
	close(pipes_fd[i][READ]);
}

void		pipe_sequence(t_exec **exec, int **pipes_fd, int nb_pipes)
{
	int		i;
	int		pid;
	t_bsh	*bsh;
	char	**cmd;
	int		builtin;

	bsh = get_bsh();
	i = -1;
	while (++i < nb_pipes)
		if (pipe(pipes_fd[i]) < 0)
			exit(EXIT_FAILURE);
	i = -1;
	while (++i < nb_pipes + 1)
	{
		cmd = lst_to_tab((*exec)->word_list, (*exec)->word_count);
		expand_words(bsh->exp, cmd);
		if ((builtin = cmd_is_builtin(cmd)) > -1)
		{
			connect_pipe(pipes_fd, nb_pipes, i);
			run_builtin(builtin, cmd);
		}
		else
		{
			if ((pid = fork()) < 0)
				exit(EXIT_FAILURE);
			if (!pid)
			{
				connect_pipe(pipes_fd, nb_pipes, i);
				run_binary(*exec, bsh->env, cmd);
			}
		}
		ft_free_string_tab(&cmd);
		*exec = (*exec)->next;
	}
}

void	simple_command(t_exec *exec, char **cmd)
{
	int	is_builtin;

	if ((is_builtin = cmd_is_builtin(cmd)) > -1)
		run_builtin(is_builtin, cmd);
	else
		run_binary(exec, get_bsh()->env, cmd);
}
