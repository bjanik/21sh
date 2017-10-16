#include "builtins.h"

t_env	*env_to_lst(char **environ)
{
	t_env	*env;
	int	i;

	env = NULL;
	i = 0;
	while (environ[i])
	{
		if (!env)
			env = create_node(environ[i++], 1);
		else
			push_back_env(&env, environ[i++], 1);
	}
	return (env);
}

t_env		*create_node(char *env_var, int export)
{
	t_env	*env;
	char	**splitted_env_var;
	
	if (!env_var)
		return (NULL);
	splitted_env_var = ft_strsplit(env_var, '=');
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		perror("malloc failed");
	if (!(env->var_name = ft_strdup(splitted_env_var[0])))
		perror("malloc failed");
	if (splitted_env_var[1])
	{
		if (!(env->var_value = ft_strdup(splitted_env_var[1])))
			perror("malloc failed");
	}
	else if (!(env->var_value = ft_strdup("")))
		perror("malloc failed");
	env->next = NULL;
	env->exportable = export;
	ft_free_string_tab(&splitted_env_var);
	return (env);
}

void		push_back_env(t_env **env, char *env_var, int export)
{
	t_env	*ptr;

	if (*env)
	{
		ptr = *env;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = create_node(env_var, export);
	}
	else
		*env = create_node(env_var, export);
}

int	display_env(t_env *env)
{
	while (env)
	{
		//if (env->exportable)
			ft_printf("%s=%s  exportable = %d\n", env->var_name, env->var_value, env->exportable);
		env = env->next;
	}
	return (0);
}

t_env	*ft_getenv(t_env *env, char *name)
{
	t_env	*e;

	e = env;
	while (e)
	{
		if (!ft_strcmp(e->var_name, name))
			return (e);
		e = e->next;
	}
	return (NULL);
}
