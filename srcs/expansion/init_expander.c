/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 18:41:39 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/24 18:43:01 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_expander	*init_expander(t_env *env)
{
	t_expander *exp;

	if ((exp = (t_expander*)malloc(sizeof(t_expander))) == NULL)
		exit (EXIT_FAILURE);
	exp->state = INIT1;
	exp->event = START1;
	exp->env = env;
	if (!(exp->buffer = (char *)ft_memalloc(INITIAL_SIZE + 1)))
		exit (EXIT_FAILURE);
	exp->buffer_len = 0;
	exp->tmp = NULL;
	return (exp);
}
