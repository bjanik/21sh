/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 15:50:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/08 19:22:55 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		run_builtin(t_exec *exec)
{
	t_bsh	*bsh;

	bsh = get_bsh();
	if (handle_redirection(exec))
	{
		bsh->exit_status = 1;
		return ;
	}
	bsh->exit_status = g_builtins[exec->is_builtin].builtin(&(bsh->env),
			exec->cmd);
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

static void	launch_builtins(t_exec *ex, t_bsh *bsh)
{
	int	i;

	i = -1;
	while (++i <= bsh->pipes->nb_pipes)
	{
		if (ex->is_builtin > -1 && (!ex->next || ex->next->is_builtin == -1))
		{
			save_fds(bsh->saved_fds);
			connect_processes_pipes(bsh->pipes, i);
			run_builtin(ex);
			restore_fds(bsh->saved_fds);
		}
		ex = ex->next;
	}
}

void		pipe_sequence(t_exec **exec, t_pipes *pipes)
{
	int		i;
	int		pid[100];
	int		k;
	t_bsh	*bsh;
	t_exec	*ex;

	bsh = get_bsh();
	k = 0;
	i = -1;
	create_pipes(pipes);
	ex = *exec;
	while (++i <= pipes->nb_pipes)
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
				connect_processes_pipes(pipes, i);
				close_pipes_fds(pipes);
				run_binary(*exec, bsh->env);
			}
		}
		if (i < pipes->nb_pipes)
			*exec = (*exec)->next;
	}
	launch_builtins(ex, bsh);
	close_pipes_fds(bsh->pipes);
	while (k)
		waitpid(pid[--k], &bsh->exit_status, 0);
	restore_custom_attr(bsh->term);
}

static int	simple_command(t_exec *exec, t_env *env, t_term *term,
		t_expander *exp)
{
	int	exit_status;
	int	pid;

	exit_status = 0;
	exec->cmd = lst_to_tab(exec->word_list, exec->word_count);
	expand_words(exp, exec->cmd);
	if ((exec->is_builtin = cmd_is_builtin(exec->cmd)) > -1)
	{
		save_fds(get_bsh()->saved_fds);
		run_builtin(exec);
		restore_fds(get_bsh()->saved_fds);
	}
	else
	{
		if ((pid = fork()) < 0)
			exit(EXIT_FAILURE);
		if (!pid)
		{
			restore_initial_attr(term);
			run_binary(exec, env);
		}
	}
	(exec->is_builtin == -1) ? waitpid(pid, &exit_status, 0) : 0;
	restore_custom_attr(term);
	return (exit_status);
}

void	execution(t_bsh *bsh)
{
	t_exec	*exec;

	exec = bsh->exec;
	handle_heredocs(bsh->exec);
	while (exec && exec->word_list)
	{
		if (get_pipes_fd(exec, bsh->pipes))
		{
			pipe_sequence(&exec, bsh->pipes);
			clear_pipes(bsh->pipes);
		}
		else
			bsh->exit_status = simple_command(exec, bsh->env, bsh->term,
					bsh->exp);
		if (WIFEXITED(bsh->exit_status) && exec->is_builtin == -1)
			bsh->exit_status = WEXITSTATUS(bsh->exit_status);
		if (WIFSIGNALED(bsh->exit_status) && exec->is_builtin == -1)
			bsh->exit_status = WTERMSIG(bsh->exit_status) + 128;
		if ((exec->cmd_separator == AND_IF && bsh->exit_status) ||
				(exec->cmd_separator == OR_IF && !bsh->exit_status))
		{
			exec = exec->next;
			while (exec && exec->cmd_separator == PIPE)
				exec = exec->next;
		}
		if (exec)
			exec = exec->next;
	}
}
