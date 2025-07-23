#include "minishell.h"

char **tokens_to_args(t_tokenizer *tokens)
{
    int count = 0;
    t_tokenizer *tmp = tokens;

    while (tmp && tmp->op == NOT_OP)
    {
        count++;
        tmp = tmp->next;
    }

    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;

    tmp = tokens;
    int i = 0;
    while (tmp && tmp->op == NOT_OP)
    {
        args[i++] = ft_strdup(tmp->str);
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

void free_args(char **args)
{
    int i = 0;
    if (!args)
        return;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}
