/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 14:39:46 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/06 16:11:15 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_transit	g_expander[MAX_STATE1][MAX_EVENT1] = {
	{{STD1, skip},
		{DQUOTE1, skip},
		{QUOTE1, skip},
		{STD1, handle_dollar},
		{STD1, handle_tilde},
		{STD1, handle_bckslsh},
		{STD1, append}},

	{{STD1, NULL},
		{DQUOTE1, skip},
		{QUOTE1, skip},
		{STD1, handle_dollar},
		{STD1, handle_tilde},
		{STD1, handle_bckslsh},
		{STD1, append}},

	{{STD1, NULL},
		{STD1, skip},
		{DQUOTE1, append},
		{DQUOTE1, handle_dollar},
		{DQUOTE1, append},
		{DQUOTE1, handle_bckslsh},
		{DQUOTE1, append}},

	{{QUOTE1, NULL},
		{QUOTE1, append},
		{STD1, skip},
		{QUOTE1, append},
		{QUOTE1, append},
		{QUOTE1, append},
		{QUOTE1, append}},
};

static void	save_expanded_word(t_expander *exp, char **word, int i)
{
	char	*tmp;

	tmp = word[i];
	if (ft_strcmp(exp->buffer, word[i]))
	{
		if (!(word[i] = ft_strdup(exp->buffer)))
			exit(EXIT_FAILURE);
		ft_strdel(&tmp);
	}
	ft_bzero(exp->buffer, exp->buffer_size);
	exp->buffer_len = 0;
	get_event_exp(exp);
}

void		realloc_exp_buffer(t_expander *exp)
{
	char	*tmp;

	tmp = exp->buffer;
	if (!(exp->buffer = (char*)malloc((exp->buffer_size * 2)
			* sizeof(char))))
		exit(EXIT_FAILURE);
	exp->buffer_size *= 2;
	ft_bzero(exp->buffer, exp->buffer_size);
	ft_strcpy(exp->buffer, tmp);
	free(tmp);
}

inline void	append(t_expander *exp)
{
	if (exp->buffer_len == exp->buffer_size)
		realloc_exp_buffer(exp);
	exp->buffer[exp->buffer_len++] = *(exp->tmp);
}

inline void	skip(t_expander *exp)
{
	(void)exp;
}

inline void	handle_bckslsh(t_expander *exp)
{
	exp->tmp++;
	exp->buffer[exp->buffer_len++] = *(exp->tmp);
}

void		expand_words(t_expander *exp, char **word_tab)
{
	char	*s;
	int		i;

	i = -1;
	while (word_tab[++i])
	{
		exp->state = INIT1;
		exp->event = START1;
		exp->tmp = word_tab[i];
		s = word_tab[i];
		while (*(exp->tmp))
		{
			g_expander[exp->state][exp->event].p_transit(exp);
			(exp->state != INIT1) ? exp->tmp++ : 0;
			exp->state = g_expander[exp->state][exp->event].new_state;
			if (*(exp->tmp) == '\0')
				break ;
			get_event_exp(exp);
		}
		save_expanded_word(exp, word_tab, i);
	}
}
