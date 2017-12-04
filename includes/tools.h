/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:58:23 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/04 19:28:38 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
# define TOOLS_H

# define WORD 0
# define NEWLINE 1
# define IO_NUMBER 2
# define DLESS 3
# define DGREAT 4
# define LESSAND 5
# define GREATAND 6
# define LESS 7
# define GREAT 8
# define AND_IF 9
# define OR_IF 10
# define SEMI 11
# define AND 12
# define PIPE 13

/*enum e_tokens {
				DGREAT,
				DLESS,
				GREAT,
				LESS,
				AND_IF,
				OR_IF,
				PIPE,
				LESS_AND,
				GREAT_AND,
				SEMI,
				NEWLINE,
				IO_NUMBER,
				WORD,
};*/

typedef struct		s_token
{
	char			*token;
	int				type;
	int				pushed;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;
#endif
