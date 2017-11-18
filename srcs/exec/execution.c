/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 15:50:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/18 16:51:13 by bjanik           ###   ########.fr       */
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
	if (exec->cmd)
		bsh->exit_status = g_builtins[exec->is_builtin].builtin(&(bsh->env),
			exec->cmd);
}

static int	simple_command(t_exec *exec, t_env *env, t_term *term,
		t_expander *exp)
{
	int	exit_status;

	exit_status = 0;
	exec->cmd = expand_words(exp, exec);
	expand_filenames(exp, exec);
	if ((exec->is_builtin = cmd_is_builtin(exec->cmd)) > -1)
	{
		save_fds(get_bsh()->saved_fds);
		run_builtin(exec);
		restore_fds(get_bsh()->saved_fds);
	}
	else
	{
		if ((g_pid = fork()) < 0)
			exit(EXIT_FAILURE);
		if (!g_pid)
		{
			restore_initial_attr(term);
			run_binary(exec, env);
		}
	}
	signal(SIGINT, SIG_IGN);
	(exec->is_builtin == -1) ? waitpid(g_pid, &exit_status, 0) : 0;
	signal(SIGINT, sigint_handler);
	restore_custom_attr(term);
	return (exit_status);
}

static void	get_exit_status(t_bsh *bsh, t_exec *exec)
{
	if (WIFEXITED(bsh->exit_status) && exec->is_builtin == -1)
		bsh->exit_status = WEXITSTATUS(bsh->exit_status);
	if (WIFSIGNALED(bsh->exit_status) && exec->is_builtin == -1)
		bsh->exit_status = WTERMSIG(bsh->exit_status) + 128;
}

void		execution(t_bsh *bsh)
{
	t_exec	*exec;

	exec = bsh->exec;
	handle_heredocs(bsh->exec);
	while (exec)
	{
		if (get_pipes_fd(exec, bsh->pipes))
		{
			pipe_sequence(&exec, bsh->pipes);
			clear_pipes(bsh->pipes);
		}
		else
			bsh->exit_status = simple_command(exec, bsh->env, bsh->term,
					bsh->exp);
		get_exit_status(bsh, exec);
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
