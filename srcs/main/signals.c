/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 13:38:26 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/03 14:42:19 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

/*void	sigint_handler(int signum)
{
	t_bsh	*bsh;

	bsh = get_bsh();
	if (g_pid == 0)
	{
		write(STDOUT, "\n", 1);
		print_prompt(bsh->term, BOLD_CYAN);
	}
	else
		kill(getpid(), SIGKILL);
}*/
