/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 14:39:25 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/03 18:17:17 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int	del_newline_token(t_lexer *lexer, t_token *tokens[], t_input *input)
{
	t_token	*tk;

	tk = tokens[1]->prev;
	if (!ft_strcmp(tokens[1]->token, "\\\n"))
	{
		if (tk == NULL)
		{
			ft_strdel(&tokens[0]->token);
			ft_memdel((void**)&tokens[0]);
			tokens[0] = lexer->token_list[0];
			tokens[1] = lexer->token_list[1];
			return (0);
		}
		tk->next = lexer->token_list[0];
		lexer->token_list[0]->prev = tk;
		ft_strdel(&tokens[1]->token);
		ft_memdel((void**)&tokens[1]);
		tokens[1] = lexer->token_list[1];
		return (0);
	}
	lexer->state = STD;
	tk->next = NULL;
	ft_strdel(&tokens[1]->token);
	ft_memdel((void**)&tokens[1]);
	tokens[1] = tk;
	input->buffer[--input->buffer_len] = '\0';
	return (0);
}

static int	concat_tokens(t_lexer *lexer, t_token *tokens[], t_input *input)
{
	t_token	*tk;

	if (!ft_strcmp(tokens[1]->token, "\\\n"))
		return (del_newline_token(lexer, tokens, input));
	if (lexer->token_list[0]->type == NEWLINE)
	{
		tokens[1]->next = lexer->token_list[0];
		tokens[1] = lexer->token_list[1];
		return (0);
	}
	tk = lexer->token_list[0];
	input->buffer[--input->buffer_len] = '\0';
	tokens[1]->token = ft_strjoin_free(tokens[1]->token,
		lexer->token_list[0]->token, 3);
	tokens[1]->pushed = 0;
	if (lexer->token_list[0]->next)
	{
		tokens[1]->next = lexer->token_list[0]->next;
		lexer->token_list[0] = lexer->token_list[0]->next;
		tokens[1] = lexer->token_list[1];
		ft_memdel((void**)&tk);
	}
	else
		ft_memdel((void**)&lexer->token_list[0]);
	return (0);
}

static void	update_input_buffer(t_input *input)
{
	input->buffer_len += ft_strlen(input->buf_tmp);
	ft_strcat(input->buf_tmp, input->buffer);
	ft_strdel(&input->buffer);
	input->buffer = input->buf_tmp;
}

int		handle_unclosed_quotes(t_lexer *lex, t_input *input, int *ret,
		t_token *tokens[])
{
	while (*ret == END_IS_OP || *ret == UNCLOSED_QUOTES)
	{
		(*ret == END_IS_OP) ? del_newline_token(lex, tokens, input) : 0;
		input->buf_tmp = input->buffer;
		if (!(input->buffer = (char*)ft_memalloc((input->buffer_size + 1)
				* sizeof(char))))
			ft_error_msg("Malloc failed");
		display_basic_prompt(input->term);
		if ((*ret = wait_for_input(input, UNCLOSED_QUOTES) == UNEXPECTED_EOF))
			return (handle_unexpected_eof(input, &tokens[0]));
		if (input->type == REGULAR_INPUT)
			return (CATCH_SIGINT);
		//dprintf(input->fd, "prev_state = %d\n", lexer_prev_state);
		lexer(lex, input->buffer, lex->state);
		*ret = parser(NULL, lex->token_list[0], NO_SAVE_EXEC);
		if ((*ret == UNCLOSED_QUOTES || *ret == ACCEPTED) &&
				tokens[1]->type == WORD)
			concat_tokens(lex, tokens, input);
		else if (*ret != SYNTAX_ERROR)
		{
			tokens[1]->next = lex->token_list[0];
			tokens[1] = lex->token_list[1];
		}
		else if (*ret == SYNTAX_ERROR)
			clear_token_list(&lex->token_list[0]);
		lex->token_list[0] = NULL;
		lex->token_list[1] = NULL;
		update_input_buffer(input);
	}
	return (0);
}
