#include "builtins.h"

static int	export_usage(char invalid_opt)
{
	ft_putstr_fd("bsh: export: -", STDERR);
	ft_putchar_fd(invalid_opt, STDERR);
	ft_putendl_fd(": invalid option", STDERR);
	ft_putendl_fd("export: usage: export [-n] [name[=value] ...] or export -p",
			STDERR);
	return (1);
}

int			ft_export(t_env **env, char **cmd)
{
	int		i;
	char	options[3];
	char	wrong_opt;
	t_env	*var;

	i = 0;
	ft_bzero(options, 3);
	if (cmd && cmd[0] && !cmd[1])
		return (display_env(*env));
	while (cmd[++i] && cmd[i][0] == '-' && ft_strcmp("--", cmd[i]))
	{
		if (!cmd[i][1])
		{
			ft_putendl_fd("bsh: export: `-': not a valid identifier", STDERR);
			return (1);
		}
		if ((wrong_opt = check_arg_opt(cmd[i] + 1, "np", options)))
			return (export_usage(wrong_opt));
	}
	if (IS_OPTION(options, 'n'))
	{
		while (cmd[i])
		{
			if ((var = ft_getenv(*env, cmd[i++])))
				var->exportable = 0;
		}
	}
	while (cmd[i])
		set_var(env, cmd[i++], EXPORT_VAR);
	return (0);
}
