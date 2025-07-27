#include "minishell.h"

void terminate_with_error(t_glb *glb, const char *msg, int exit_code)
{
    (void)glb;
    perror(msg);
    exit(exit_code);
}
void ft_close(t_glb *glb, int fd)
{
    (void)glb;
    if (close(fd) == -1)
        perror("close");
}
char **ast_to_args(t_ast *cmd)
{
    t_tokenizer *tmp;
    char **args;
    int count = 0;
    int i = 0;

    if (!cmd || !cmd->cmd_line)
        return NULL;

    tmp = cmd->cmd_line;
    while (tmp)
    {
        if (tmp->op == CMD)
            count++;
        tmp = tmp->next;
    }

    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;

    tmp = cmd->cmd_line;
    while (tmp)
    {
        if (tmp->op == CMD)
            args[i++] = ft_strdup(tmp->str);
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

void left_pipe(t_glb *glb, t_ast *cmd, int fd[2], int *exit_status)
{
    pid_t pid;
    char **args;

    args = ast_to_args(cmd);
    if (!args)
    {
        fprintf(stderr, "Error: args is NULL\n");
        return;
    }

    pid = fork();
    if (pid < 0)
        perror("fork");
    else if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execute_command(args, &(glb->env));
        exit(0);
    }
    else
    {
        close(fd[1]);
        waitpid(pid, exit_status, 0);
    }
}

void right_pipe(t_glb *glb, t_ast *cmd, int fd[2], int *exit_status)
{
    char **args;
	(void)exit_status;
    args = ast_to_args(cmd);

    if (!args)
        terminate_with_error(glb, "args extraction failed", EXIT_FAILURE);

    ft_close(glb, fd[1]);
    if (dup2(fd[0], STDIN_FILENO) == -1)
        terminate_with_error(glb, "dup2", EXIT_FAILURE);
    ft_close(glb, fd[0]);

    execute_command(args, &(glb->env));
    free_args(args);

    exit(0);
}
