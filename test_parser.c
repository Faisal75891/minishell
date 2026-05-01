#include "include/minishell.h"

/*
ls -la | grep minishell | wc -l
cat < input.txt | sort > out.txt
echo "a b" '$HOME' "$HOME"
grep foo << EOF
| ls
ls >
echo "unterminated
*/
void print_parsed(t_parsed_result *parsed)
{
    int i;
    int j;
    int k;
    const char *redir_label;
    const char *redir_symbol;
    if (!parsed)
    {
        printf("Parse result: NULL\n");
        return ;
    }
    printf("\n========== PARSER RESULT ==========\n");
    printf("Command count: %d\n", parsed->command_count);
    printf("Parser error : %d\n", parsed->command_error);
    printf("===================================\n");
    i = 0;
    while (i < parsed->command_count)
    {
        printf("\n[Command %d]\n", i);
        printf("  argc: %d\n", parsed->commands[i].argc);
        printf("  argv:\n");
        if (!parsed->commands[i].argv || parsed->commands[i].argc == 0)
            printf("    (none)\n");
        else
        {
            j = 0;
            while (parsed->commands[i].argv[j])
            {
                printf("    [%d] %s\n", j, parsed->commands[i].argv[j]);
                j++;
            }
        }
        printf("  redirections: %d\n", parsed->commands[i].redirections_count);
        if (!parsed->commands[i].redirections
            || parsed->commands[i].redirections_count == 0)
            printf("    (none)\n");
        else
        {
            k = 0;
            while (k < parsed->commands[i].redirections_count)
            {
                redir_label = "UNKNOWN";
                redir_symbol = "?";
                if (parsed->commands[i].redirections[k].type == TOK_REDIR_IN)
                {
                    redir_label = "INPUT";
                    redir_symbol = "<";
                }
                else if (parsed->commands[i].redirections[k].type == TOK_REDIR_OUT)
                {
                    redir_label = "OUTPUT";
                    redir_symbol = ">";
                }
                else if (parsed->commands[i].redirections[k].type == TOK_REDIR_APPEND)
                {
                    redir_label = "APPEND";
                    redir_symbol = ">>";
                }
                else if (parsed->commands[i].redirections[k].type == TOK_HEREDOC)
                {
                    redir_label = "HEREDOC";
                    redir_symbol = "<<";
                }
                printf("    [%d] %s (%s) -> %s\n",
                    k,
                    redir_label,
                    redir_symbol,
                    parsed->commands[i].redirections[k].target
                    ? parsed->commands[i].redirections[k].target
                    : "(null)");
                k++;
            }
        }
        printf("  heredoc fd: %d\n", parsed->commands[i].heredoc);
        i++;
    }
    printf("\n======== END PARSER RESULT ========\n");
}

int main(int argc, char **argv, char **envp)
{
    t_parsed_result *parsed;
    t_lex_result    *lex;
    t_shell         *shell;
    char			*cases[] = {
        "ls -la | grep minishell | wc -l",
        "cat < input.txt | sort > out.txt",
        "echo \"a b\" \'$HOME\' \"$HOME\"", // EXPANSION NOT HANDLED YET :/
        "grep foo << EOF",
         "| ls",
         "ls >",
        "echo \"unterminated",
    };

    (void)argc;
    shell = malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		write(2, "env copy failed\n", 16);
		return (1);
	}
	shell->last_status = 0;
    lex = init_lexer();
    if (!lex)
        return (1);
    int i = 0;
    while (i < 7)
    {
        printf("case: %s\n", cases[i]);
        tokenize_lexer(cases[i], lex);
        parsed = parser(lex, shell);
        print_parsed(parsed);
        clear_lexer(lex);
        i++;
    }
    


    for (int i = 0; argv[i]; i++)
        printf("%s\n", argv[i]);
    return (0);
}
