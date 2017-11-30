/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 14:39:46 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/28 13:37:17 by bjanik           ###   ########.fr       */
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
	if (!(word[i] = ft_strdup(exp->buffer)))
		ft_error_msg("Expand failed\n");
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
		ft_error_msg("Realloc_exp_buffer failed\n");
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
	if (*(exp->tmp) != '\n')
		exp->buffer[exp->buffer_len++] = *(exp->tmp);
}

static void	expand(t_expander *exp)
{
	while (*(exp->tmp))
	{
		g_expander[exp->state][exp->event].p_transit(exp);
		(exp->state != INIT1) ? exp->tmp++ : 0;
		exp->state = g_expander[exp->state][exp->event].new_state;
		get_event_exp(exp);
	}
}

char	**expand_words(t_expander *exp, t_exec *exec)
{
	int		i;
	char	**word_tab;
	t_list	*words;

	i = -1;
	words = exec->word_list;
	word_tab = NULL;
	if (!exec->word_list)
		return (NULL);
	if (!(word_tab = (char**)malloc(sizeof(char*) * (exec->word_count + 1))))
		ft_error_msg("Malloc failed\n");
	word_tab[exec->word_count] = NULL;
	while (words)
	{
		exp->state = INIT1;
		exp->event = START1;
		exp->tmp = words->content;
		expand(exp);
		save_expanded_word(exp, word_tab, ++i);
		words = words->next;
	}
	return (word_tab);
}

void	expand_filenames(t_expander *exp, t_exec *exec)
{
	t_redir	*rd;

	rd = exec->redir_list;
	while (rd)
	{
		if (rd->filename)
		{
			exp->tmp = rd->filename;
			exp->state = INIT1;
			exp->event = START1;
			expand(exp);
			ft_strdel(&rd->filename);
			if (!(rd->filename = ft_strdup(exp->buffer)))
				ft_error_msg("Malloc failed\n");
			exp->buffer_len = 0;
			ft_bzero(exp->buffer, exp->buffer_size);
		}
		rd = rd->next;
	}
}
