#include "minishell.h"

static int count_args(t_tokenizer *tokens)
{
    int count;
    int skip_next;

    count = 0;
    skip_next = 0;
    while (tokens)
    {
        if (skip_next)
        {
            skip_next = 0;
            tokens = tokens->next;
            continue;
        }
        if (tokens->op == GREAT || tokens->op == GREAT_GREAT
            || tokens->op == LESS || tokens->op == LESS_LESS)
            skip_next = 1;
        else if (tokens->op == NOT_OP)
            count++;
        tokens = tokens->next;
    }
    return count;
}


char **tokens_to_args(t_tokenizer *tokens)
{
    char **args;
    int i;
    int skip_next;

    args = malloc(sizeof(char *) * (count_args(tokens) + 1));
    if (!args)
        return NULL;
    i = 0;
    skip_next = 0;
    while (tokens)
    {
        if (skip_next)
        {
            skip_next = 0;
            tokens = tokens->next;
            continue;
        }
        if (tokens->op == GREAT || tokens->op == GREAT_GREAT
            || tokens->op == LESS || tokens->op == LESS_LESS)
            skip_next = 1;
        else if (tokens->op == NOT_OP)
        {
            args[i] = ft_strdup(tokens->str);
            i++;
        }
        tokens = tokens->next;
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
