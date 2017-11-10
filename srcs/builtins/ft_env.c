#include "bsh.h"

static int      env_usage(char invalid_opt)
{
        ft_putstr_fd("bsh: env: -", STDERR);
        ft_putchar_fd(invalid_opt, STDERR);
        ft_putendl_fd(": invalid option", STDERR);
        ft_putendl_fd("env: usage: env [-ui] [name[=value] ...]",
                        STDERR);
        return (1);
}

int		size_env(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

char		**env_to_tab(t_env *env)
{
	char	**tab;
	int		i;

	i = 0;
	tab = NULL;
	if (!(tab = (char**)malloc((size_env(env) + 1) * sizeof(char*))))
		exit(EXIT_FAILURE);
	while (env)
	{
		if (env->exportable)
		{
			if (!(tab[i] = (char*)malloc(sizeof(char) *
				(ft_strlen(env->var_value) + ft_strlen(env->var_name) + 2))))
			exit(EXIT_FAILURE);
			ft_strcpy(tab[i], env->var_name);
			ft_strcat(tab[i], "=");
			ft_strcat(tab[i++], env->var_value);
		}
		env = env->next;
	}
	tab[i] = NULL;
	return (tab);
}

/*t_env		*modified_env(t_env *env, char **cmd, int index)
{
	t_env	*mod_env;
	t_env	*ptr;
	int	i;
	
	mod_env = NULL;
	while (env)
	{	
		i = index;
		while (cmd[i] && ft_str_iscap(cmd[i]))
		{
			if (!ft_strcmp(env->var_value, cmd[i++]))
			{
				env = env->next;
				break ;
			}
		}	
		if (i == index)
		{
			mod_env = malloc(sizeof(t_env));
			mod_env->var_name = ft_strdup(env->var_name);
			mod_env->var_value = ft_strdup(env->var_value);
			mod_env->next = NULL;
			env = env->next;
		}
	}
}*/

int		ft_env(t_env **env, char **cmd)
{

	char	wrong_opt;
	char	options[3];
	int	i;

	i = 0;
	ft_bzero(options, 3);
	if (cmd && cmd[0] && !cmd[1])
		return (display_env(*env));
	while (cmd[++i] && cmd[i][0] == '-' && ft_strcmp("--", cmd[i]))
	{
		if (!cmd[i][1])
			options[0] = 'i';
		if ((wrong_opt = check_arg_opt(cmd[i] + 1, "iu", options)))
			return (env_usage(wrong_opt));
	}
	if (IS_OPTION(options, 'i'))
	{
		while (cmd[i] && ft_strchr(cmd[i], '='))
			set_var(&get_bsh()->mod_env, cmd[i++], EXPORT_VAR);
		
	}
	else if (IS_OPTION(options, 'u'))
	{
	//	while (cmd[i])
	//	{
	//		while (
	//	}
	}
	display_env(get_bsh()->mod_env);
	return (0);
}
