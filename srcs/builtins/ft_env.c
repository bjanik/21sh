#include "builtins.h"

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
		perror("malloc");
	while (env)
	{
		if (env->exportable)
		{
			if (!(tab[i] = (char*)malloc(sizeof(char) * (ft_strlen(env->var_value) + ft_strlen(env->var_name) + 2))))
				perror("malloc");
			ft_strcpy(tab[i], env->var_name);
			ft_strcat(tab[i], "=");
			ft_strcat(tab[i++], env->var_value);
		}
		env = env->next;
	}
	tab[i] = NULL;
	return (tab);
}
