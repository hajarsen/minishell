#include "minishell.h"


static void cleanup_loop(t_tokenizer **tokens, t_ast **ast, char **args, char *input)
{
    if (args)
        free_args(args);
    if (*tokens)
    {
        free_tokens(input, *tokens);
        *tokens = NULL;
    }
    if (*ast)
    {
        free_ast(*ast);
        *ast = NULL;
    }
}

static void execute_child(t_tokenizer *tokens, char **args)
{
    char **envp;
    char *path;
    int exit_status = 0;

    if (execute_redirections(tokens))
        exit(1);

    if (execute_builtin(args, &glb_list()->env, &exit_status) == 1)
        exit(exit_status);

    path = get_cmd_path(args[0], glb_list()->env);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(args[0], 2);
        ft_putchar_fd('\n', 2);
		glb_list()->exit_status = 127;
        exit(127);
    }

    envp = envlist_to_array(glb_list()->env);
    if (!envp)
    {
        perror("envlist_to_array");
        free(path);
        exit(1);
    }

    execve(path, args, envp);
    perror("execve");
    free(path);
    free_strs(envp);
    exit(1);
}

static int fork_and_execute(t_tokenizer *tokens, char **args)
{
    pid_t pid;
    int exit_status = 0;

    pid = fork();
    if (pid < 0)
        perror("fork");
    else if (pid == 0)
        execute_child(tokens, args);
    else
    {
        waitpid(pid, &exit_status, 0);
        if (WIFEXITED(exit_status))
            exit_status = WEXITSTATUS(exit_status);
		glb_list()->exit_status = exit_status;
    }
    return (exit_status);
}

int main(int ac, char **av, char **env)
{
    t_tokenizer *tokens = NULL;
    t_ast *ast = NULL;
    char *input = NULL;
    char **args = NULL;
    int exit_status = 0;

    (void)ac;
    (void)av;

    setup_signals();
    glb_list()->env = NULL;
    init_env_var(&glb_list()->env, env);

    while (1)
    {
        input = readline("\033[1;32m➜\033[0m\033[1;36m Minishell $> \033[0m");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (input_error(input))
        {
            free(input);
            continue;
        }
        add_history(input);

        tokens = tokenizer(input);
        if (!tokens)
        {
            free(input);
            continue;
        }
        if (check_parsing_errors(tokens))
        {
            free_tokens(input, tokens);
            continue;
        }
        expanding(&tokens);
        redirection_infos(tokens);
		process_heredocs(tokens, glb_list()->env, &exit_status);
        ast = ast_builder(tokens);
        if (!ast)
        {
            free_tokens(input, tokens);
            continue;
        }

        set_signal_handler(ast);

        args = tokens_to_args(tokens);
        if (args && args[0])
        {
            int status = fork_and_execute(tokens, args);
            if (status == -1)
            {
                cleanup_loop(&tokens, &ast, args, input);
                free_env(glb_list()->env);
                exit(EXIT_FAILURE);
            }
            exit_status = status;
        }
        cleanup_loop(&tokens, &ast, args, input);
        args = NULL;
        input = NULL;
    }
    free_env(glb_list()->env);
    glb_list()->env = NULL;
    return (exit_status);
}


