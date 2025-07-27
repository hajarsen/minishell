#include "minishell.h"

void free_redirections(t_redirections *rdc)
{
    if (!rdc)
        return;
    free(rdc->str);
    free(rdc);
}

void free_tokenizer_list(t_tokenizer *tokens)
{
    t_tokenizer *tmp;
    t_here_doc  *hd_tmp;
    t_here_doc  *hd;

    while (tokens)
    {
        tmp = tokens->next;
        free(tokens->str);

        hd = tokens->hd;
        while (hd)
        {
            hd_tmp = hd->next;
            free(hd->str);
            free(hd);
            hd = hd_tmp;
        }
        free(tokens);
        tokens = tmp;
    }
}

void free_ast(t_ast *ast)
{
    if (!ast)
        return;

    free_ast(ast->left);
    free_ast(ast->right);

    if (ast->cmd)
        free(ast->cmd);

    if (ast->rdc)
        free_redirections(ast->rdc);

    free(ast);
}
