#include "minishell.h"

void	remove_empty_quote(char *str, int start, int end)
{
	int	i;

	i = 0;
	while (str[start + i + 1] != 0)
	{
		str[start + i] = str[start + i + 1];
		i++;
	}
	start = end - 1;
	i = 0;
	while (str[start + i + 1] != 0)
	{
		str[start + i] = str[start + i + 1];
		i++;
	}
	str[ft_strlen(str) - 2] = 0;
}

int	quote_handling(char *str)
{
	int		i;
	char	q;

	i = 0;
	while (str[i] != 0)
	{
		q = is_quote(str[i]);
		if (q != 0)
		{
			start = i;
			i++;
			while (q != is_quote(str[i]))
				i++;
			remove_empty_quote(str, start, i);
			i -= 2;
		}
		i++;
	}
	return (0);
}

/*for other $? $!...*/
int	valid_expanding(char *str, int *len)
{
	int			i;

	i = 1;
	if (!ft_isalnum(str[1]))
		return (0);
	if (ft_isdigit(str[1]))
	{
		*len = 2;
		return (1);
	}
	while (ft_isalnum(str[i]))
		i++;
	*len = i;
	return (1);
}

/*void	env_var(t_tokenizer *token)
{
	int			i;
	char		*env_value;
	int			len;

	i = 0;
	while (token->str[i] != 0)
	{
		if (token->str[i] == '\'')
		{
			i++;
			while (token->str[i] != '\'')
				i++;
		}
		if (token->str[i] == '$' && valid_expanding(token->str + i, &len))
		{
			env_value = check_env(ft_substr(token->str, i + 1, len - 1));
			token->str = re_alloc(token->str, i, len, env_value);
		}
		i++;
	}
}*/

static void insert_split_tokens(t_tokenizer *token, char **split_values)
{
    t_tokenizer *current = token;
    int j;

    j = 1;
    while (split_values[j])
    {
        t_tokenizer *new_token = malloc(sizeof(t_tokenizer));
        new_token->str = ft_strdup(split_values[j]);
        new_token->op = NOT_OP;
        new_token->next = current->next;
        current->next = new_token;
        current = new_token;
        j++;
    }
}

static int process_variable_in_str(t_tokenizer *token, int i)
{
    int len;
    char *env_value;
    char **split_values;

    if (!valid_expanding(token->str + i, &len))
        return (i);

    env_value = check_env(ft_substr(token->str, i + 1, len - 1));
    if (env_value && ft_strchr(env_value, ' '))
    {
        split_values = ft_split(env_value, ' ');
        free(token->str);
        token->str = ft_strdup(split_values[0]);
        insert_split_tokens(token, split_values);
        for (int j = 0; split_values[j]; j++)
            free(split_values[j]);
        free(split_values);
    }
    else
        token->str = re_alloc(token->str, i, len, env_value);
    return (i + len);
}

void env_var(t_tokenizer *token_list)
{
    t_tokenizer *token = token_list;
    int i;

    while (token)
    {
        if (token->op == NOT_OP && token->str)
        {
            i = 0;
            while (token->str[i])
            {
                if (token->str[i] == '\'')
                {
                    i++;
                    while (token->str[i] && token->str[i] != '\'')
                        i++;
                }
                else if (token->str[i] == '$')
                    i = process_variable_in_str(token, i);
                else
                    i++;
            }
        }
        token = token->next;
    }
}

void	expanding(t_tokenizer *token)
{
	while (token != NULL)
	{
		if (token->op == LESS_LESS)
			token->hd = here_doc(token);
		else
			token->hd = NULL;
		if (token->op == NOT_OP)
			env_var(token);
		if (token->op == NOT_OP)
			quote_handling(token->str);
		token = token->next;
	}
}
