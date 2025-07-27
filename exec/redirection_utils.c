#include "minishell.h"

void init_redirect_fds(t_tokenizer *tokens)
{
    while (tokens)
    {
        tokens->redirect.file_fd = -1;
        tokens = tokens->next;
    }
}

int redirection_infos(t_tokenizer *tokens)
{
    while (tokens && tokens->next)
    {
        int fd = -1;

        if (tokens->op == GREAT)
            fd = open(tokens->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (tokens->op == GREAT_GREAT)
            fd = open(tokens->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (tokens->op == LESS)
            fd = open(tokens->next->str, O_RDONLY);
        else if (tokens->op == LESS_LESS)
            fd = open_heredoc_and_write_pipe(tokens->next, glb_list()->env, NULL);

        tokens->next->redirect.file_fd = fd;

        tokens = tokens->next;
    }
    return 0;
}



void	close_redirection_fds(t_tokenizer *token)
{
	while (token)
	{
		if (token->redirect.file_fd > 2)
			close(token->redirect.file_fd);
		token = token->next;
	}
}

int	here_doc_present(t_tokenizer *tokens)
{
	while (tokens)
	{
		if (tokens->op == LESS_LESS)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
