#include "minishell.h"

int	input_error(char *input)
{
	int		i;
	int		error;
	char	c;

	i = 0;
	while (input[i])
	{
		error = 1;
		c = input[i];
		if (is_quote(c))
		{
			i++;
			while (input[i] && input[i] != c)
				i++;
			if (c == input[i])
				error = 0;
			if (error == 1)
			{
				printf("minishell: Unexpected token\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	check_parsing_errors(t_tokenizer *token)
{
	while (token != NULL)
	{
		if (token->op != NOT_OP && ((token->next != NULL
					&& token->next->op != NOT_OP) || token->next == NULL))
		{
			if (token->op == PIPE)
				printf("Minishell: syntax error near unexpected token `|\'\n");
			else
				printf
				("Minishell: syntax error near unexpectedtoken \'newline\'\n");
			return (1);
		}
		token = token->next;
	}
	return (0);
}
