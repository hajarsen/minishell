#include "minishell.h"

int is_builtin_cd(char *cmd)
{
    return (cmd && ft_strcmp(cmd, "cd") == 0);
}

int is_pipeline(t_tokenizer *tokens)
{
    while (tokens)
    {
        if (tokens->op == PIPE)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

int is_in_pipeline(t_tokenizer *tokens)
{
    return is_pipeline(tokens);
}
static void signal_handler(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

static pid_t handle_pipeline_child_processes(t_ast *pipe_cmd, int fd[2], t_glb *glb, int *exit_status)
{
    pid_t pid1 = fork();

    (void)exit_status;
    if (pid1 == 0)
    {
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);

        char **args = tokens_to_args(pipe_cmd->left->cmd_line);
        execute_command(args, &glb->env);
        free_args(args);
        exit(0);
    }
    return pid1;
}

static void extract_exit_status(int status, int *exit_status)
{
    if (WIFEXITED(status))
        *exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        *exit_status = 128 + WTERMSIG(status);
    else
        *exit_status = 1;
}

void execute_pipeline_command(t_ast *cmd, t_glb *glb, int *exit_status)
{
    int fd[2];
    int status;
    pid_t pid1, pid2;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = handle_pipeline_child_processes(cmd, fd, glb, exit_status);
    pid2 = fork();
    if (pid2 == 0)
    {
        close(fd[1]);
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd[0]);

        char **args = tokens_to_args(cmd->right->cmd_line);
        execute_command(args, &glb->env);
        free_args(args);
        exit(0);
    }

    signal_handler();
    close(fd[0]);
    close(fd[1]);
    waitpid(pid2, &status, 0);
    extract_exit_status(status, exit_status);
    waitpid(pid1, NULL, 0);
}
