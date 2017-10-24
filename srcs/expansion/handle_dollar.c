/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 18:43:22 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/24 20:30:47 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static void	get_process_pid(t_expander *exp)
{
	char	*s;

	if (!(s = ft_itoa(get_bsh()->pid)))
		exit(EXIT_FAILURE);
	ft_strcat(exp->buffer, s);
	free(s);
	exp->tmp++;
}

static void	get_exit_status(t_expander *exp)
{
	char	*s;

	if (!(s = ft_itoa(get_bsh()->exit_status)))
		exit(EXIT_FAILURE);
	ft_strcat(exp->buffer, s);
	free(s);
	exp->tmp++;
}

static void	get_shell_name(t_expander *exp)
{
	ft_strcat(exp->buffer, get_bsh()->shell_name);
	exp->tmp++;
}

void		handle_dollar(t_expander *exp)
{
	t_env	*env_var;
	int		len;
	char	*s;

	len = 0;
	s = exp->tmp + 1;
	if (!*s)
		append(exp);
	else if (*s == '?')
		get_exit_status(exp);
	else if (*s == '$')
		get_process_pid(exp);
	else if (*s == '0')
		get_shell_name(exp);
	else
	{
		while (ft_isdigit(*(s + len)) || ft_isalpha(*(s + len)))
			len++;
		s = ft_strndup(exp->tmp + 1, len);
		if ((env_var = ft_getenv(exp->env, s)))
		{
			ft_strcat(exp->buffer, env_var->var_value);
			exp->buffer_len += ft_strlen(env_var->var_value);
			free(s);
		}
		exp->tmp += len;
	}
}
