#include "include/minishell.h"

int	execute_commands(t_parsed_result *parsed_result, t_shell *shell);


void print_parsed(t_parsed_result *parsed)
{
    if (!parsed)
    {
        printf("=================>Parse result: NULL<=================\n");
        return ;
    }
    printf("\n========== PARSER RESULT START ==========\n");
    printf("Command count: %d\n", parsed->command_count);
    printf("Parser error : %d\n", parsed->command_error);
    printf("\n======== END PARSER RESULT ========\n");
}



int main(int argc, char **argv, char **envp)
{
    t_parsed_result *parsed = NULL;
    t_lex_result    *lex;
    t_shell         *shell;
    char *cases[] = {
        "echo \"foo\" | grep foo",
        "ls >",
        "cat < input.txt | sort > out.txt",
        "ls -la | grep minishell | wc -l",
        "echo \"a b\" '$HOME' \"$HOME\"",
        "| ls",
        "echo \"unterminated",
        "echo hello",
        "printf \"hello\nhi\nwassup\n\" | grep hi",
        "printf \"b\na\nc\n\" | sort | tr -d '\n'",
        "echo hello > out.txt",
        "echo first > out.txt",
        "echo second >> out.txt",
        "echo \"Hello i am writing to out.txt using cat\" | cat > out.txt",
        "printf \"b\na\n\" | sort > out.txt",
        "echo hello > no_such_dir/out.txt",
        "ls |",
        "echo \"unterminated",
        "echo \"a b\" '$HOME' \"$HOME\"",
        "echo hello > file1 > file2",
        "|",
        "ls || wc",
        "cat >",
        "cat < | grep",
        "ls >>"
    };

    (void)argc;
    (void)argv;
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
    while (i < 25)
    {
        printf("==================================================================================\n");
        printf("Case: %s\n", cases[i]);
        tokenize_lexer(cases[i], lex);
        if (lex->error == 0)
        {
            parsed = parser(lex, shell);
            if (parsed->command_error == 0)
            {
                execute_commands(parsed, shell);
            }
            free_parser(parsed);
        }
        clear_lexer(lex);
        i++;
        printf("==================================================================================\n");
    }
    free(lex);
    free_split(shell->env);
    free(shell);
    return (0);
}
