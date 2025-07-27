#include "minishell.h"

static t_ast *create_ast_node(t_operator type, t_tokenizer *cmd_tokens)
{
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
        return NULL;
    node->type = type;
    node->cmd_line = cmd_tokens;
    node->cmd = NULL;
    node->rdc = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static t_tokenizer *find_operator_token(t_tokenizer *tokens, t_operator op)
{
    while (tokens)
    {
        if (tokens->op == op)
            return tokens;
        tokens = tokens->next;
    }
    return NULL;
}

t_ast *ast_builder(t_tokenizer *tokens)
{
    t_ast *root = NULL;
    t_tokenizer *op_token;

    if (!tokens)
        return NULL;

    op_token = find_operator_token(tokens, PIPE);

    if (op_token)
    {
        t_tokenizer *left_tokens = tokens;
        t_tokenizer *right_tokens = op_token->next;

        op_token->next = NULL;

        root = malloc(sizeof(t_ast));
        if (!root)
            return NULL;

        root->type = PIPE;
        root->cmd_line = NULL;
        root->left = ast_builder(left_tokens);
        root->right = ast_builder(right_tokens);
        root->rdc = NULL;
        root->cmd = NULL;
    }
    else
    {
        root = create_ast_node(CMD, tokens);
    }

    return root;
}
