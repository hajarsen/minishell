#include "minishell.h"

static int is_valid_identifier(const char *s)
{
	int i;

	i = 0;
	if (!s || (!(ft_isalpha(s[i]) || s[i] == '_')))
		return (0);
	while (s[i])
	{
		if (s[i] == '=')
			break;
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void print_exported_vars(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
}

static void update_or_add_export(char *arg, t_env **env_list)
{
	char *eq;
	char *name;
	char *value;

	eq = ft_strchr(arg, '=');
	name = NULL;
	value = NULL;
	if (eq)
	{
		name = ft_substr(arg, 0, eq - arg);
		value = ft_strdup(eq + 1);
	}
	else
		name = ft_strdup(arg);
	if (name)
		insert_env_node(env_list, create_env_node(name, value));
}

int ft_export(char **args, t_env **env_list)
{
	int i;

	i = 1;
	if (!args[1])
	{
		print_exported_vars(*env_list);
		return (0);
	}
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
			update_or_add_export(args[i], env_list);
		i++;
	}
	return (0);
}

