#include "include/minishell.h"

#include <ctype.h>
#include <string.h>

int execute_commands(t_parsed_result *parsed_result, t_shell *shell);

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

static size_t	count_cases(char **cases)
{
    size_t n = 0;
    while (cases && cases[n])
        n++;
    return (n);
}

static int	parse_size_t(const char *s, size_t *out)
{
    unsigned long long	v;
    char				*end;

    if (!s || !*s)
        return (0);
    while (isspace((unsigned char)*s))
        s++;
    v = strtoull(s, &end, 10);
    if (end == s)
        return (0);
    while (isspace((unsigned char)*end))
        end++;
    if (*end != '\0')
        return (0);
    *out = (size_t)v;
    return (1);
}

static void	usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s [--list] [--only N] [--from A --to B] [--grep STR] [--tokens]\n"
        "  --list        Print case indices and commands\n"
        "  --only N      Run only case N\n"
        "  --from A      Start index (inclusive, default 0)\n"
        "  --to B        End index (exclusive, default: case_count)\n"
        "  --grep STR    Run only cases containing STR (substring)\n"
        "  --tokens      Print lexer tokens for each case\n",
        prog);
}

static int	run_one_case(t_shell *shell, t_lex_result *lex, char *cmd,
        size_t idx, int tokens)
{
    t_parsed_result	*parsed;

    printf("==================================================================================\n");
    printf("[%zu] %s\n", idx, cmd);

    shell->last_status = 0;

    tokenize_lexer(cmd, lex);
    if (lex->error != 0)
    {
        printf("lexer_error: %d\n", lex->error);
        clear_lexer(lex);
        return (1);
    }

    parsed = parser(lex, shell);
    if (!parsed || parsed->command_error > 0)
    {
        printf("parser_error: %d\n", parsed ? parsed->command_error : -1);
        free_parser(parsed);
        clear_lexer(lex);
        return (1);
    }

    if (tokens)
        print_tokens(lex);

    execute_commands(parsed, shell);
    printf("exit_status: %d\n", shell->last_status);

    free_parser(parsed);
    clear_lexer(lex);
    return (0);
}

int	main(int argc, char **argv, char **envp)
{
    t_shell			*shell;
    t_lex_result	*lex;

    char			*cases[] = {
        "echo \"a b\" '$HOME' \"$HOME\"",
        "echo \"Hello HereDoc\n  \" EOF | cat << EOF",
        "echo \"Hello HereDoc with expansion \nEOF\" | cat << EOF",
        "echo \"$HOME\" | cat << EOF",
        "echo \"$HOME\" | cat << 'EOF'",
        "echo \"$HOME\" | cat << \"EOF\"",
        "echo \"foo\" | grep foo",
        "ls >",
        "echo hello > file1 > file2",
        "cat < file1.txt | sort > file3.txt",
        "ls -la | grep minishell | wc -l",
        "| ls",
        "echo \"unterminated",
        "echo hello",
        "printf \"hello\nhi\nwassup\n\" | grep hi",
        "printf \"b\na\nc\n\" | sort | tr -d '\n'",
        "echo hello > file5.txt",
        "echo first > file5.txt",
        "echo second >> file5.txt",
        "echo \"Hello i am writing to file5.txt using cat\" | cat > file5.txt",
        "printf \"b\na\n\" | sort > file5.txt",
        "echo hello > no_such_dir/file5.txt",
        "ls |",
        "echo \"unterminated",
        "echo \"a b\" '$HOME' \"$HOME\"",
        "|",
        "ls || wc",
        "cat >",
        "cat < | grep",
        "ls >>",

        "echo '$HOME'",
        "echo \"$HOME\"",
        "echo pre\"$HOME\"post",
        "echo pre\"abc\"post",
        "echo a\"$HOME\"b'$USER'",
        "echo \"'$HOME' $HOME\"",
        "echo '\"$HOME\"'",
        "echo \"$HOME\" '/$USER'",
        "echo \"$UNSET_VAR\"",
        "echo $?",
        "echo $$",

        "cat << EOF",
        "cat << 'EOF'",
        "cat << \"EOF\"",
        NULL
    };

    size_t			case_count = count_cases(cases);
    size_t			from = 0;
    size_t			to = case_count;
    size_t			only = (size_t)-1;
    const char		*grep = NULL;
    int				list_only = 0;
    int				tokens = 0;
    int				failures = 0;

    for (int ai = 1; ai < argc; ai++)
    {
        if (strcmp(argv[ai], "--help") == 0)
            return (usage(argv[0]), 1);
        else if (strcmp(argv[ai], "--list") == 0)
            list_only = 1;
        else if (strcmp(argv[ai], "--tokens") == 0)
            tokens = 1;
        else if (strcmp(argv[ai], "--grep") == 0 && ai + 1 < argc)
            grep = argv[++ai];
        else if (strcmp(argv[ai], "--only") == 0 && ai + 1 < argc)
        {
            if (!parse_size_t(argv[++ai], &only))
                return (fprintf(stderr, "Invalid --only value\n"), 1);
        }
        else if (strcmp(argv[ai], "--from") == 0 && ai + 1 < argc)
        {
            if (!parse_size_t(argv[++ai], &from))
                return (fprintf(stderr, "Invalid --from value\n"), 1);
        }
        else if (strcmp(argv[ai], "--to") == 0 && ai + 1 < argc)
        {
            if (!parse_size_t(argv[++ai], &to))
                return (fprintf(stderr, "Invalid --to value\n"), 1);
        }
        else
            return (fprintf(stderr, "Unknown/invalid arg: %s\n", argv[ai]), usage(argv[0]), 1);
    }

    if (list_only)
    {
        for (size_t i = 0; i < case_count; i++)
            printf("[%zu] %s\n", i, cases[i]);
        return (0);
    }

    if (only != (size_t)-1)
    {
        from = only;
        to = only + 1;
    }
    if (from > case_count) from = case_count;
    if (to > case_count) to = case_count;
    if (to < from) to = from;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        return (1);

    shell->env = copy_env(envp);
    if (!shell->env)
        return (write(2, "env copy failed\n", 16), free(shell), 1);

    shell->last_status = 0;

    lex = init_lexer();
    if (!lex)
        return (free_split(shell->env), free(shell), 1);

    for (size_t i = from; i < to; i++)
    {
        if (grep && !strstr(cases[i], grep))
            continue;
        failures += run_one_case(shell, lex, cases[i], i, tokens);
    }

    printf("==================================================================================\n");
    printf("Done. cases_total=%zu range=[%zu,%zu) failures=%d\n",
        case_count, from, to, failures);

    free(lex);
    free_split(shell->env);
    free(shell);
    return (failures ? 1 : 0);
}
