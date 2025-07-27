#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/libft.h"
# define TRUE 1
# define FALSE 2
#define SHELL_CHILD_PID_FILE "/tmp/minishell_child_pid"


typedef enum e_operator
{
	LESS_LESS,
	GREAT_GREAT,
	LESS,
	GREAT,
	PIPE,
	QUOTE,
	DOUBLE_QUOTE,
	OR,
	AND,
	CMD,
	WILDCARD,
	NEXT,
	NOT_OP,
	ENV_CASE,
}				t_operator;

typedef enum e_quote
{
	NO_SPACE,
	SPACE_BEFORE,
	SPACE_AFTER,
	SPACE_BEFORE_AFTER,
	NO_QUOTE,
	THERES_QUOTE,
}			t_quote;


typedef struct s_here_doc
{
	char				*str;
	struct s_here_doc	*next;
}t_here_doc;

typedef struct	s_env_list
{
	char				*str;
	int					i;
	struct s_env_list	*next;
}				t_env_list;

typedef struct s_redirections
{
	t_operator		type;
	char			*str;
	int				file_fd;
	t_quote			qt;

}			t_redirections;

typedef struct s_tokenizer
{
	int					i;
	int					*quotes_index;
	char				*str;
	t_quote				quote_state;
	t_operator			op;
	t_operator			env_case;
	t_env_list			*env_list;
	t_here_doc			*hd;
	t_redirections		redirect;
	struct s_tokenizer	*next;
}				t_tokenizer;

typedef struct s_ast
{
	t_operator 		type;
	t_redirections	*rdc;
	t_tokenizer		*cmd_line;
	char			*cmd;
	struct s_ast	*right;
	struct s_ast	*left;
}				t_ast;

typedef struct s_garbage_collector
{
	t_ast			*tree_node;
	t_tokenizer		*token;
	t_redirections	*rdc;
	int             exit_status;

}				t_garbage_collector;

typedef struct s_env
{
	char	*name;
	char	*value;
	struct s_env *next;

}t_env;

typedef struct s_glb
{
	t_ast			*ast;
	t_env			*env;
	t_tokenizer		*tokens;
	t_redirections	*rdr;
	int 			exit_status;

}t_glb;


t_glb	*glb_list(void);
void	print_env(t_env *env_list);
void	print_node(t_ast	*ast);
void	print_tree(t_ast	*ast);
void	print_tokenizer(t_tokenizer *tokens);
void	print_op(t_operator op);

/* FREE_EXIT*/

extern t_garbage_collector		*g_gc;
void	free_tokens(char *input, t_tokenizer *tokens);
void	free_env(t_env *env);

t_env 	*save_env(char **env); // already exist in main , could delete this line

/* TOKENIZER */

int		is_operator(char *str);
char	*alloc_quote_help(char *str, int *i);
void	fill_the_node_str(t_tokenizer *node, int i, char *token);
void	fill_the_node_op(t_tokenizer *node, int op, int i);
t_tokenizer *tokenizer(char *input);
void 	expanding(t_tokenizer **token);
char	is_quote(char c);

/*EXPANDING*/
char		*check_env(char *str);
char		*re_alloc(char *str, int *start, int len, char  *env_value);
t_here_doc	*here_doc(t_tokenizer *token);
void		expand_nq(t_tokenizer **token, int *i);
t_tokenizer	*tokenizer_for_expanding(char *input);
void		tokenize_the_envar(t_tokenizer **token);
int			to_retokenize(t_tokenizer **token);
void		save_index(t_tokenizer *token);
/* AST_ PASRER */
t_ast	*ast_builder(t_tokenizer *token);

/* ERRORS */

int		input_error(char *input);
int		check_parsing_errors(t_tokenizer *token);
/*BUILTINS*/

void 	pwd(int *exit_status);
void 	echo(char **args, int *exit_status);
void	ft_exit(char **args);
void	builtin_unset(char **args, t_env **env_list);
void 	ft_exit(char **args);
long long my_ft_atoll(const char *str, int *overflow);
int 	ft_env(char **args, t_env *env_list);
int 	ft_cd(char **args, t_env **env_list);
int 	ft_export(char **args, t_env **env_list);
int 	is_builtin(const char *cmd);
int 	execute_builtin(char **args, t_env **env_list, int *exit_status);
char	**tokens_to_args(t_tokenizer *tokens);
void	free_args(char **args);
int		execute_command(char **args, t_env **env);
char	*str_concat_three(const char *a, const char *b, const char *c);
void	free_strs(char **strs);
char	*get_cmd_path(char *cmd, t_env *env_list);
/*ENVIRONMENT VARIABLES*/
int		ft_strcmp(const char *s1, const char *s2);
void	init_env_var(t_env **env_list, char **envp);
t_env	*create_env_node(char *name, char *value);
void	insert_env_node(t_env **env_list, t_env *new_node);
char	*get_env_value(char *name, t_env *env);
void	free_env(t_env *env);
int		update_env_var(t_env *env_list, t_env *new_node);
void	update_shell_lvl(t_env **env_list);
char	**envp_to_env_vector(t_env *env_list);
char	**envlist_to_array(t_env *env_list);

int	redirection_infos(t_tokenizer *token);
void	close_redirection_fds(t_tokenizer *token);
int	here_doc_present(t_tokenizer *tokens);
int	open_heredoc_and_write_pipe(t_tokenizer *token, t_env *env, int *exit_status);
//int open_heredoc_and_write_pipe(t_tokenizer *token, char **env, void *data);
void	process_heredocs(t_tokenizer *tokens, t_env *env, int *exit_status);
int	execute_redirections(t_tokenizer *tokens);
void signal_handler_general(int signum);
void signal_handler_input(int signum);
void signal_handler_heredoc(int signum);
void setup_signals(void);
void set_signal_handler(t_ast *ast);
void free_ast(t_ast *ast);
void free_tokenizer_list(t_tokenizer *tokens);
void free_redirections(t_redirections *rdc);
void init_redirect_fds(t_tokenizer *tokens);
char **ast_to_args(t_ast *cmd);
void	execute_pipeline_command(t_ast *cmd, t_glb *glb, int *exit_status);
void terminate_with_error(t_glb *glb, const char *msg, int exit_code);
void	save_exit_status(t_glb *glb, int status_code);
void ft_close(t_glb *glb, int fd);
void	left_pipe(t_glb *glb, t_ast *cmd, int fd[2], int *exit_status);
void	right_pipe(t_glb *glb, t_ast *cmd, int fd[2], int *exit_status);
int is_in_pipeline(t_tokenizer *tokens);
int is_pipeline(t_tokenizer *tokens);
int is_builtin_cd(char *cmd);

#endif
