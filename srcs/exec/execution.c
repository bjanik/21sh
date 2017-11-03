/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 15:50:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/03 16:42:44 by bjanik           ###   ########.fr       */
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
	bsh->exit_status = g_builtins[builtin].builtin(&(bsh->env), cmd);
}

static void	connect_pipes(int **pipes_fd, int nb_pipes, int i)
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

void	save_fds(int *saved_fd)
{
	saved_fd[0] = dup(STDIN);
	saved_fd[1] = dup(STDOUT);
	saved_fd[2] = dup(STDERR);
}

void	restore_fds(int *saved_fd)
{
	dup2(saved_fd[0], STDIN);
	dup2(saved_fd[1], STDOUT);
	dup2(saved_fd[2], STDERR);
}

/*static void		launch_binaries(t_exec **exec, int **pipes_fd, int nb_pipes,
		t_env *env, int i)
{
	while (++i <= nb_pipes)
	{
		(*exec)->cmd = lst_to_tab((*exec)->word_list, (*exec)->word_count);
		expand_words(bsh->exp, (*exec)->cmd);
		if (((*exec)->is_builtin = cmd_is_builtin((*exec)->cmd)) > -1)
			;
		else
		{
			((pid[k] = fork()) < 0) ? exit(EXIT_FAILURE) : 0;
			if (!pid[k++])
			{
				restore_initial_attr(bsh->term);
				connect_pipes(pipes_fd, nb_pipes, i);
				close_pipes_fds(pipes_fd, nb_pipes);
				run_binary(*exec, env);
			}
		}
		*exec = (*exec)->next;
	}
}*/

static void	launch_builtins(t_exec *ex, int **pipes_fd, int nb_pipes,
		t_bsh *bsh)
{
	int	i;

	i = -1;
	while (++i <= nb_pipes)
	{
		save_fds(bsh->saved_fds);
		if (ex->is_builtin > -1 && (!ex->next || ex->next->is_builtin == -1))
		{
			connect_pipes(pipes_fd, nb_pipes, i);
			run_builtin(ex->is_builtin, ex->cmd);
		}
		restore_fds(bsh->saved_fds);
		ex = ex->next;
	}
}

void		pipe_sequence(t_exec **exec, int **pipes_fd, int nb_pipes)
{
	int		i;
	int		pid[100];
	int		k;
	t_bsh	*bsh;
	t_exec	*ex;

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
				connect_pipes(pipes_fd, nb_pipes, i);
				close_pipes_fds(pipes_fd, nb_pipes);
				run_binary(*exec, bsh->env);
			}
		}
		if (i < nb_pipes)
			*exec = (*exec)->next;
	}
	launch_builtins(ex, pipes_fd, nb_pipes, bsh);
	close_pipes_fds(pipes_fd, nb_pipes);
	while (k)
		waitpid(pid[--k], &bsh->exit_status, 0);
	restore_custom_attr(bsh->term);
}

void	execution(t_bsh *bsh)
{
	t_exec	*exec;
	int		**pipes_fd;
	int		nb_pipes;
	int		pid;

	pipes_fd = NULL;
	exec = bsh->exec;
	while (exec)
	{
		if ((pipes_fd = get_pipes_fd(exec, &nb_pipes)))
			pipe_sequence(&exec, pipes_fd, nb_pipes);
		else
		{
			exec->cmd = lst_to_tab(exec->word_list, exec->word_count);
			expand_words(bsh->exp, exec->cmd);
			if ((exec->is_builtin = cmd_is_builtin(exec->cmd)) > -1)
				run_builtin(exec->is_builtin, exec->cmd);
			else
			{
				if ((pid = fork()) < 0)
					exit(EXIT_FAILURE);
				if (!pid)
				{
					restore_initial_attr(bsh->term);
					run_binary(exec, bsh->env);
				}
			}
			waitpid(pid, &bsh->exit_status, 0);
			restore_custom_attr(get_bsh()->term);
		}
		if (WIFEXITED(bsh->exit_status))
			bsh->exit_status = WEXITSTATUS(bsh->exit_status);
		if ((exec->cmd_separator == AND_IF && bsh->exit_status) ||
				(exec->cmd_separator == OR_IF && !bsh->exit_status))
			exec = exec->next;
		if (exec)
			exec = exec->next;
		}
	}
