#include "minishell.h"

/*void	free_tokens(char *input, t_tokenizer *tokens)
{
	t_tokenizer	*temp;
	void		*tmp;

	while (tokens != NULL)
	{
		if (tokens->op == LESS_LESS)
		{
			tokens->hd = tokens->hd->next;
			while (tokens->hd != NULL)
			{
				free(tokens->hd->str);
				tokens->hd->str = NULL;
				tmp = tokens->hd;
				tokens->hd = tokens->hd->next;
				free(tmp);
			}
		}
		free(tokens->str);
		temp = tokens;
		tokens = tokens->next;
		free(temp);
	}
	free(input);
}*/
void free_tokens(char *input, t_tokenizer *tokens)
{
    if (tokens)
        free_tokenizer_list(tokens);
    if (input)
        free(input);
}
// void	free_env(t_env *env)
// {
// 	void	*tmp;

// 	while (env != NULL)
// 	{
// 		free(env->name);
// 		free(env->value);
// 		tmp = env;
// 		env = env->next;
// 		free(tmp);
// 	}
// }
