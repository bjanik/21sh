#ifndef BUILTINS_H
# define BUILTINS_H
# include "libft.h"
# include "errno.h"

typedef struct		s_env
{
	char		*var_name;
	char		*var_value;
	short int	exportable;
	struct s_env	*next;
}			t_env;

typedef struct		s_builtins
{
	char		*b_name;
	int		(*builtins)(t_env **, char **);
}			t_builtins;

t_env			*dup_env(char **environ);
t_env			*create_node(char *env_var);
void			push_back_env(t_env **env, char *env_var);
int			env_size(t_env *env);
int			display_env(t_env *env);
char			**env_to_tab(t_env *env);

/*static t_builtins g_builtins[] = {
        { "cd", ft_cd },
        { "echo", ft_echo },
        { "env", ft_env},
        { "setenv", ft_setenv },
        { "unsetenv", ft_unsetenv},
	{ "export", ft_export},
	{ "history", ft_history},
	{ NULL, NULL},
};*/

#endif
