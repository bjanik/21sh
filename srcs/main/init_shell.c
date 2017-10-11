#include "libft.h"

t_env	*dup_env(char **environ)
{
	t_env	*env;
	int	i;

	env = NULL;
	i = 0;
	while (environ[i])
	{
		if (!env)
			env = create_node(environ[i++]);
		else
			push_back_env(&env, environ[i++]);
	}
	return (env);
}

t_env		*create_node(char *env_var)
{
	t_env	*env;
	char	**splitted_env_var;
	
	if (!env_var)
		return (NULL);
	splitted_env_var = ft_strsplit(env_var, '=');
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		ft_perror("malloc failed");
	if (!(env->var_name = ft_strdup(splitted_env_var[0])))
		ft_perror("malloc failed");
	if (splitted_env_var[1])
	{
		if (!(env->var_value = ft_strdup(splitted_env_var[1])))
			ft_perror("malloc failed");
	}
	else if (!(env->var_value = ft_strdup("")))
		ft_perror("malloc failed");
	env->next = NULL;
	env->exportable = 1;
	ft_free_string_tab(&splitted_env_var);
	return (env);
}
