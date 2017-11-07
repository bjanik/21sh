/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 17:13:23 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/06 16:11:13 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H
# include "tools.h"
# include "history.h"
# include "builtins.h"
# define INITIAL_SIZE 4096

enum			e_state1
{
	INIT1,
	STD1,
	DQUOTE1,
	QUOTE1,
	MAX_STATE1,
};

enum			e_event1
{
	START1,
	EV_DQUOTE1,
	EV_QUOTE1,
	EV_DOLLAR,
	EV_TILDE,
	EV_BACKSLASH1,
	EV_REG_CHAR1,
	MAX_EVENT1,
};

typedef struct	s_expander
{
	int			state;
	int			event;
	t_env		*env;
	char		*buffer;
	int			buffer_len;
	int			buffer_size;
	char		*tmp;
}				t_expander;

typedef struct	s_transit
{
	int			new_state;
	void		(*p_transit)(t_expander *exp);
}				t_transit;

t_expander		*init_expander(t_env *env);
void			expand_words(t_expander *exp, char **word_tab);
void			append(t_expander *exp);
void			skip(t_expander *exp);
void			handle_tilde(t_expander *exp);
void			handle_dollar(t_expander *exp);
void			handle_bckslsh(t_expander *exp);
void			get_event_exp(t_expander *exp);
void			realloc_exp_buffer(t_expander *exp);

t_transit		g_expander[MAX_STATE1][MAX_EVENT1];
#endif
