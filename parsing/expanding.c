#include "minishell.h"

void	remove_quote(char *str, int start, int end)
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

int	quote_handling(t_tokenizer *token)
{
	int		i;
	char	q;
	int		start;
	int		j;

	i = 0;
	j = 0;
	while (token->str[i] != 0)
	{
		q = is_quote(token->str[i]);
		if (q != 0 && i == (token->quotes_index[j] - (j / 2)))
		{
			start = i;
			i++;
			j++;
			while (q != is_quote(token->str[i])
				|| i != (token->quotes_index[j] - j / 2))
				i++;
			remove_quote(token->str, start, i);
			j++;
			i -= 2;
		}
		i++;
	}
	return (0);
}

t_tokenizer	**env_var(t_tokenizer **token)
{
	int			i;
	char		c;

	i = 0;
	while ((*token)->str[i] != 0)
	{
		c = (*token)->str[i];
		if (is_quote((*token)->str[i]))
		{
			i++;
			while ((*token)->str[i] != c)
				i++;
		}
		expand_nq(token, &i);
		i++;
	}
	if (to_retokenize(token) == 1)
		tokenize_the_envar(token);
	save_index(*token);
	return (token);
}

void	expanding(t_tokenizer **token)
{
	t_tokenizer	**temp;

	temp = token;
	while ((*temp) != NULL)
	{
		if ((*temp)->op == LESS_LESS)
			(*temp)->hd = here_doc((*temp));
		else
			(*temp)->hd = NULL;
		if ((*temp)->op == NOT_OP)
			temp = env_var(temp);
		if ((*temp)->op == NOT_OP)
			quote_handling((*temp));
		if ((*temp) == NULL)
			break ;
		temp = &(*temp)->next;
	}
}
