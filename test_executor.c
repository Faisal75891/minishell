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

void	print_tokens(t_lex_result *lex)
{
    t_token	*current;
    int		idx;

    idx = 0;
    current = lex->head;
    while (current)
    {
        printf("  [%d] type=%d quote=%d word=[%s]\n",
            idx, current->type, current->quote, current->word);
        current = current->next;
        idx++;
    }
}

int main(int argc, char **argv, char **envp)
{
	t_parsed_result *parsed = NULL;
	t_lex_result    *lex;
	t_shell         *shell;
	char *cases[] = {
		"echo \"a b\" '$HOME' \"$HOME\"",
		"echo \"Hello HereDoc\n  \" EOF | cat << EOF",
		"echo \"Hello HereDoc with expansion \nEOF\" | cat << EOF",
		"echo \"$HOME\" | cat << EOF",
		"echo \"$HOME\" | cat << 'EOF'",
		"echo \"$HOME\" | cat << \"EOF\"",
		"echo \"foo\" | grep foo",
		"ls >",
		"cat < input.txt | sort > out.txt",
		"ls -la | grep minishell | wc -l",
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
	char *expansion_cases[] = {
		"echo '$HOME'", // Doesn't expand
		"echo \"$HOME\"", // expands
		"echo pre\"$HOME\"post", // expands
		"echo pre\"abc\"post", // quotes should be removed :/
		"echo a\"$HOME\"b'$USER'", // only HOME expands
		"echo \"'$HOME' $HOME\"", // Both expand
		"echo '\"$HOME\"'", // doesnt expand
		"echo \"$HOME\" '/$USER'", // only HOME expands
		"echo \"$UNSET_VAR\"", // idk lol
		"echo $?",
		"echo $$",
	};

	char	*here_doc_cases[] = {
		"cat << EOF", // should epand vars
		"cat << 'EOF'", // shouldnt expand vars
		"cat << \"EOF\"",
		//---- unquoted ----
		// /home/faisal
		// ---- single quoted ----
		// $HOME
		// ---- double quoted ----
		// $HOME
	};
	printf("%p\n", cases);
	printf("%p\n", expansion_cases);
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
	while (i < 3)
	{
		printf("==================================================================================\n");
		printf("Case: %s\n", here_doc_cases[i]);
		tokenize_lexer(here_doc_cases[i], lex);
		if (lex->error == 0)
		{
			parsed = parser(lex, shell);
			if (!parsed || parsed->command_error > 0)
			{
				free_parser(parsed);
				i++;
				continue ;
			}
			print_tokens(lex);
			execute_commands(parsed, shell);
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
