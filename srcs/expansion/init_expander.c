/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 18:41:39 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/06 15:09:50 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_expander	*init_expander(t_env *env)
{
	t_expander *exp;

	if ((exp = (t_expander*)malloc(sizeof(t_expander))) == NULL)
		exit(EXIT_FAILURE);
	exp->state = INIT1;
	exp->event = START1;
	exp->env = env;
	exp->buffer_size = INITIAL_SIZE + 1;
	if (!(exp->buffer = (char *)malloc(exp->buffer_size)))
		exit(EXIT_FAILURE);
	ft_bzero(exp->buffer, exp->buffer_size);
	exp->buffer_len = 0;
	exp->tmp = NULL;
	return (exp);
}
