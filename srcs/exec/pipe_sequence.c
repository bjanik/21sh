/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_sequence.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 15:08:24 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/28 11:28:00 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

/*static void	launch_builtins(t_exec *ex, t_bsh *bsh)
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
}*/

static void	pipes_loop(t_bsh *bsh, t_exec **exec, int pid[], int *k)
{
	int	i;

	i = -1;
	while (++i <= bsh->pipes->nb_pipes)
	{
		(*exec)->cmd = expand_words(bsh->exp, *exec);
		expand_filenames(bsh->exp, *exec);
		if ((pid[*k] = fork()) < 0)
			ft_error_msg("Fork failed\n");
		if (!pid[(*k)++])
		{
			connect_processes_pipes(bsh->pipes, i);
			close_pipes_fds(bsh->pipes);
			if (((*exec)->is_builtin = cmd_is_builtin((*exec)->cmd)) > -1)
			{
				launch_builtin(bsh, *exec, 0);
				exit(bsh->exit_status);
			}
			else
				run_binary(*exec, bsh->env, 0);
		}
		if (i < bsh->pipes->nb_pipes)
			*exec = (*exec)->next;
	}
}

/*static void	pipes_loop(t_bsh *bsh, t_exec **exec, int pid[], int *k)
{
	int	i;

	i = -1;
	while (++i <= bsh->pipes->nb_pipes)
	{
		(*exec)->cmd = expand_words(bsh->exp, *exec);
		expand_filenames(bsh->exp, *exec);
		if (((*exec)->is_builtin = cmd_is_builtin((*exec)->cmd)) > -1)
			;
		else
		{
			if ((pid[*k] = fork()) < 0)
				exit(EXIT_FAILURE);
			if (!pid[(*k)++])
			{
				restore_initial_attr(bsh->term);
				connect_processes_pipes(bsh->pipes, i);
				close_pipes_fds(bsh->pipes);
				run_binary(*exec, bsh->env);
			}
		}
		if (i < bsh->pipes->nb_pipes)
			*exec = (*exec)->next;
	}
}*/

void		pipe_sequence(t_exec **exec, t_pipes *pipes)
{
	int		i;
	int		pid[10000];
	int		k;
	t_bsh	*bsh;
	t_exec	*ex;

	bsh = get_bsh();
	k = 0;
	create_pipes(pipes);
	ex = *exec;
	pipes_loop(bsh, exec, pid, &k);
	close_pipes_fds(bsh->pipes);
	signal(SIGINT, SIG_IGN);
	i = 0;
	while (i <= k)
		waitpid(pid[i++], &bsh->exit_status, 0);
	signal(SIGINT, sigint_handler);
}
