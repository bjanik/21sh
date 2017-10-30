/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:06:59 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/20 16:50:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "exec.h"

int	g_successor_state[18][MAX_SYM] = {
	{0, 10, 11, 12, 13, 14, 15, 16, 17, -1, -1, 18, 19, 20, -1, -1, -1},
	{3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 21, 22, -1, -1, -1},
	{4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24, -1},
	{5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, -1, -1},
	{6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, -1, -1},
	{7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1},
	{8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, -1},
	{9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1},
	{12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 34},
	{16, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, 40, 19, 20, -1, -1, -1},
	{17, -1, -1, -1, -1, -1, -1, -1, -1, 42, -1, 43, 19, 20, -1, -1, -1},
	{34, -1, -1, -1, 44, 14, 15, 16, 17, -1, -1, 18, 19, 20, -1, -1, -1},
	{35, -1, -1, -1, -1, 45, 15, 16, 17, -1, -1, 18, 19, 20, -1, -1, -1},
	{36, -1, -1, -1, -1, 46, 15, 16, 17, -1, -1, 18, 19, 20, -1, -1, -1},
	{37, -1, -1, -1, -1, 14, 47, 16, 17, -1, -1, 18, 19, 20, -1, -1, -1},
	{39, -1, -1, -1, -1, -1, -1, -1, -1, 48, -1, 43, 19, 20, -1, -1, -1},
	{42, -1, -1, -1, -1, -1, -1, -1, -1, 48, -1, 50, 19, 20, -1, -1, -1},
	{48, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 50, 19, 20, -1, -1, -1},
};

int	g_reduce_table[NB_RULES] = {
	ACCEPT,
	PROGRAM,
	PROGRAM,
	COMPLETE_COMMAND,
	COMPLETE_COMMAND,
	LIST,
	LIST,
	AND_OR,
	AND_OR,
	AND_OR,
	PIPELINE,
	PIPELINE,
	COMMAND,
	COMMAND,
	COMMAND,
	COMMAND,
	COMMAND,
	CMD_NAME,
	CMD_WORD,
	CMD_PREFIX,
	CMD_PREFIX,
	CMD_SUFFIX,
	CMD_SUFFIX,
	CMD_SUFFIX,
	CMD_SUFFIX,
	IO_REDIRECT,
	IO_REDIRECT,
	IO_REDIRECT,
	IO_REDIRECT,
	IO_FILE,
	IO_FILE,
	IO_FILE,
	IO_FILE,
	IO_FILE,
	FILENAME,
	IO_HERE,
	HERE_END,
	SEPARATOR_OP,
	SEPARATOR_OP,
};

t_save_exec	g_save_exec_list[NB_RULES] = {
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{append_wordlist},
	{append_wordlist},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{append_wordlist},
	{append_wordlist},
	{NULL},
	{set_io_number},
	{NULL},
	{set_io_number},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{NULL},
	{set_filename},
	{set_here_end},
	{NULL},
	{NULL},
};
