/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 15:50:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/30 16:39:37 by bjanik           ###   ########.fr       */
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

void	test_pipes(int *pipe)
{
	char	buf[10];

	ft_memset(buf, 0 , 10);
	write(pipe[WRITE], "SALUT\n", 6);
	if (read(pipe[READ], buf, 6) < 0)
		ft_printf("READ failed...\n");
	ft_printf("pipe[read] = %d && pipe[write] = %d\nData read from the pipe :--> %s", pipe[READ], pipe[WRITE] , buf);
}

static void	connect_pipe(int **pipes_fd, int nb_pipes, int i)
{
	if (!i)
	{
		test_pipes(pipes_fd[0]);
		if (dup2(pipes_fd[i][WRITE], STDOUT))
		{
			perror("dup2 1");
			exit(EXIT_FAILURE);
		}
	}
	else if (i < nb_pipes)
	{
		if (dup2(pipes_fd[i - 1][READ], STDIN))
		{
			perror("dup2 2");
			exit(EXIT_FAILURE);
		}
		if (dup2(pipes_fd[i][WRITE], STDOUT))
		{
			perror("dup2 3");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		test_pipes(pipes_fd[i - 1]);
		if (dup2(pipes_fd[i - 1][READ], STDOUT))
		{
			perror("dup2 4");
			exit(EXIT_FAILURE);
		}
	}
	close_pipes_fds(pipes_fd, nb_pipes);
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
	{
		if (pipe(pipes_fd[i]) < 0)
			exit(EXIT_FAILURE);
		ft_printf("pipe_fd[%d][READ] = %d && pipes_fd[%d][WRITE] = %d\n", i, pipes_fd[i][READ], i, pipes_fd[i][WRITE]);
	}
	i = -1;
	while (++i < nb_pipes)
	{
		ft_printf("%d\t%d\n", pipes_fd[i][0], pipes_fd[i][1]);
	}
	test_pipes(pipes_fd[0]);
	i = -1;
	while (++i <= nb_pipes)
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
	close_pipes_fds(pipes_fd, nb_pipes);
	waitpid(pid, &bsh->exit_status, 0);
	restore_custom_attr(bsh->term);
}

void	simple_command(t_exec *exec, char **cmd)
{
	int	is_builtin;

	if ((is_builtin = cmd_is_builtin(cmd)) > -1)
		run_builtin(is_builtin, cmd);
	else
		run_binary(exec, get_bsh()->env, cmd);
}
