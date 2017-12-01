/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 14:39:25 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/01 14:43:09 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static void	del_newline_token(t_lexer *lexer, t_token **tokens)
{
	t_token	*tk;
	t_bsh	*bsh;

	bsh = get_bsh();
	tk = *tokens;
	lexer->state = STD;
	lexer->prev_state = STD;
	tk = (*tokens)->prev;
	tk->next = NULL;
	ft_strdel(&(*tokens)->token);
	ft_memdel((void**)tokens);
	(*tokens) = tk;
	bsh->input->buffer[--bsh->input->buffer_len] = '\0';
}

static void	concat_tokens(t_lexer *lexer, t_token *tokens[], t_input *input)
{
	t_token	*tk;

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
}

static void	update_input_buffer(t_input *input)
{
	input->buffer_len += ft_strlen(input->buf_tmp);
	ft_strcat(input->buf_tmp, input->buffer);
	ft_strdel(&input->buffer);
	input->buffer = input->buf_tmp;
}

int		handle_unexpected_eof(t_input *input, t_token *tokens[])
{
	ft_putendl_fd("bash : syntax error: unexpected end of file", STDERR);
	clear_token_list(&tokens[0]);
	ft_strdel(&input->buffer);
	input->buffer = input->buf_tmp;
	input->buf_tmp = NULL;
	input->buffer[input->buffer_len++] = '\0';
	return (UNEXPECTED_EOF);
}

int		handle_unclosed_quotes(t_lexer *lex, t_input *input, int *ret,
		t_token *tokens[])
{
	int	lexer_prev_state;

	while (*ret == END_IS_OP || *ret == UNCLOSED_QUOTES)
	{
		(*ret == END_IS_OP) ?
			del_newline_token(lex, &tokens[1]) : 0;
		input->buf_tmp = input->buffer;
		if (!(input->buffer = (char*)ft_memalloc((input->buffer_size + 1)
				* sizeof(char))))
			ft_error_msg("Malloc failed");
		display_basic_prompt(input->term);
		if ((*ret = wait_for_input(input, UNCLOSED_QUOTES) == UNEXPECTED_EOF))
			return (handle_unexpected_eof(input, &tokens[0]));
		lexer_prev_state = lex->state;
		//dprintf(input->fd, "prev_state = %d\n", lexer_prev_state);
		lexer(lex, input->buffer, lex->state);
		*ret = parser(NULL, lex->token_list[0], NO_SAVE_EXEC);
		if ((*ret == UNCLOSED_QUOTES || *ret == ACCEPTED) &&
				tokens[1]->type == WORD && lex->token_list[0]->type != NEWLINE)
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
