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
        "ls >",
        "cat << EOF > file.txt",
        "cat < input.txt | sort > out.txt",
        "grep foo << EOF",
        "ls -la | grep minishell | wc -l",
        "echo \"a b\" '$HOME' \"$HOME\"",
        "| ls",
        "echo \"unterminated",
        "echo hello",
        "printf \"hello\nhi\nwassup\n\" | grep hi",
        "printf \"b\na\nc\n\" | sort | tr -d '\n'",
        "cat < input.txt",
        "echo hello > out.txt",
        "echo first > out.txt",
        "echo second >> out.txt",
        "cat << EOF",
        "cat << EOF > out.txt",
        "cat < input.txt | sort",
        "printf \"b\na\n\" | sort > out.txt",
        "cat < missing.txt",
        "cat < missing.txt | sort > out.txt",
        "echo hello > no_such_dir/out.txt",
        "| ls",
        "ls |",
        "echo \"unterminated",
        "echo \"a b\" '$HOME' \"$HOME\"",
        "echo hello > file1 > file2",
        "cat << EOF"
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
    while (i < 7)
    {
        printf("Case: %s\n", cases[i]);
        tokenize_lexer(cases[i], lex);
        parsed = parser(lex, shell);
        execute_commands(parsed, shell);
        clear_lexer(lex);
        i++;
    }
    return (0);
}
