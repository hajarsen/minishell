#include "minishell.h"


int execute_redirections(t_tokenizer *tokens)
{
    while (tokens)
    {
        if ((tokens->op == GREAT || tokens->op == GREAT_GREAT) && tokens->next)
        {
            if (tokens->next->redirect.file_fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(tokens->next->str, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                return 1;
            }
            if (dup2(tokens->next->redirect.file_fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                return 1;
            }
        }
        else if ((tokens->op == LESS || tokens->op == LESS_LESS) && tokens->next)
        {
            if (tokens->next->redirect.file_fd < 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(tokens->next->str, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                return 1;
            }
            if (dup2(tokens->next->redirect.file_fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                return 1;
            }
        }
        tokens = tokens->next;
    }
    return 0;
}
