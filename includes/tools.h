#ifndef TOOLS_H
#define TOOLS_H

#define WORD 0
#define NEWLINE 1
#define IO_NUMBER 2
#define DLESS 3
#define DGREAT 4
#define LESSAND 5
#define GREATAND 6
#define LESS 7
#define GREAT 8
#define AND_IF 9
#define OR_IF 10
#define SEMI 11
#define AND 12
#define PIPE 13
/*typedef enum e_tokens {
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
			}e_tokens;*/

typedef struct		s_token
{
	char			*token;
	int				type;
	int				pushed;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

#endif

