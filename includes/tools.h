#ifndef TOOLS_H
#define TOOLS_H

#define WORD 257
#define ASSIGNMENT_WORD 258
#define NAME 259
#define NEWLINE 260
#define IO_NUMBER 261
#define AND_IF 262
#define OR_IF 263
#define DSEMI 264
#define DLESS 265
#define DGREAT 266
#define LESSAND 267
#define GREATAND 268
#define LESSGREAT 269
#define DLESSDASH 270
#define CLOBBER 271
#define If 272
#define Then 273
#define Else 274
#define Elif 275
#define Fi 276
#define Do 277
#define Done 278
#define Case 279
#define Esac 280
#define While 281
#define Until 282
#define For 283
#define Lbrace 284
#define Rbrace 285
#define Bang 286
#define In 287
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
			
typedef struct 		s_token
{
	char		*token;
	int		type;	
	struct s_token	*next;
}			t_token;

#endif

