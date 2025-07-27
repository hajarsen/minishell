#include "minishell.h"

char	*check_env(char *str)
{
	t_env	*env;

	if (ft_strncmp(str, "?", 1) == 0 && ft_strlen(str) == 1)
    {
        free(str);
        return (ft_itoa(glb_list()->exit_status));
    }

	env = glb_list()->env;
	while (env != 0)
	{
		if (ft_strncmp(str, env->name, ft_strlen(env->name) + 1) == 0)
		{
			free(str);
			return (env->value);
		}
		env = env->next;
	}
	free(str);
	return (NULL);
}

char	*new_alloc(char *str, char *env_value, int *start, int len)
{
	int		new_len;
	char	*new_str;
	int		new_start;

	new_len = ft_strlen(str) - len + ft_strlen(env_value);
	new_str = malloc(new_len + 1);
	ft_strlcpy(new_str, str, *start + 1);
	ft_strlcat(new_str, env_value, new_len + 1);
	new_start = ft_strlen(new_str);
	ft_strlcat(new_str, str + *start + len, new_len + 1);
	*start = new_start - 1;
	return (new_str);
}

char	*re_alloc(char *str, int *start, int len, char *env_value)
{
	char	*new_str;

	if (env_value == NULL)
	{
		ft_strlcpy(str + *start, str + *start + len, ft_strlen(str) - len + 1);
		*start -= 1;
		return (str);
	}
	new_str = new_alloc(str, env_value, start, len);
	free(str);
	return (new_str);
}

t_here_doc	*here_doc(t_tokenizer *token)
{
	char		*delemeter;
	t_here_doc	*hd;
	t_here_doc	*head;

	hd = malloc(sizeof(t_here_doc));
	head = hd;
	hd->str = token->next->str;
	delemeter = hd->str;
	hd->next = malloc(sizeof(t_here_doc));
	hd = hd->next;
	hd->str = readline("> ");
	while (ft_strncmp(delemeter, hd->str, ft_strlen(hd->str)) != 0)
	{
		hd->next = malloc(sizeof(t_here_doc));
		hd = hd->next;
		hd->str = readline("> ");
	}
	hd->next = NULL;
	return (head);
}

void	tokenize_the_envar(t_tokenizer **token)
{
	t_tokenizer	**token_temp;
	char		*str;
	t_tokenizer	**token_next;
	t_tokenizer	*token_dele;

	token_dele = *token;
	str = (*token)->str;
	token_next = &(*token)->next;
	*token = tokenizer_for_expanding((*token)->str);
	free(str);
	free(token_dele);
	token_temp = token;
	while ((*token_temp) != NULL)
	{
		token_temp = &(*token_temp)->next;
	}
	*token_temp = *token_next;
}
