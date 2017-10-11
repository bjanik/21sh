NAME = 21sh

CC = gcc

FLAGS = -Wall -Wextra

LIBFT  = libft/includes

LIB = libft/libft.a

HEADER = includes

SRC_PATH = srcs
OBJ_PATH = obj

SRC_NAME = builtins/ft_env.c \
	   builtins/ft_export.c \
	   builtins/handle_env.c \
	   exec/init_redir.c \
	   exec/init_exec.c \
	   exec/prepare_exec.c \
	   exec/redirs.c \
	   exec/redir_greatand.c \
	   line_editing/get_key.c \
	   line_editing/input.c \
	   line_editing/init_input.c \
	   line_editing/keys_alt.c \
	   line_editing/keys_arrows.c \
	   line_editing/keys_ctrl.c \
	   line_editing/keys_delete.c \
	   line_editing/keys_home_end_return.c \
	   line_editing/term.c \
	   lexer/lexer.c \
	   lexer/init_token.c \
           main.c \
           parser/parser.c \
           parser/parser_tables.c \
           parser/parser_stack.c \
	   history/history.c \

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all : $(NAME)

$(NAME) : $(OBJ)
	make -C $(LIBFT)
	$(CC) $(FLAGS) $(OBJ) $(LIB) -o $(NAME)

$(OBJ) : $(OBJ_PATH)

$(OBJ_PATH) :
	mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(FLAGS) -I$(HEADER) -I$(LIBFT) -c $< -o $@

clean :
	rm -rf $(OBJ_PATH)
	make clean -C $(LIBFT)

fclean : clean
	make fclean -C $(LIBFT)
	rm -f $(NAME)

re : fclean all

