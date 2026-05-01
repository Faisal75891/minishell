
#include "include/minishell.h"

typedef struct s_test_case
{
    const char	*name;
    const char	*input;
    int			expected_count;
    int			expected_error;
} t_test_case;

static void	print_tokens(t_lex_result *lex)
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

static int	run_case(t_lex_result *lex, const t_test_case *tc)
{
    int	ok;

    tokenize_lexer((char *)tc->input, lex);
    printf("\n== %s ==\n", tc->name);
    printf("input: %s\n", tc->input);
    printf("count=%d error=%d\n", lex->count, lex->error);
    print_tokens(lex);
    ok = (lex->count == tc->expected_count && lex->error == tc->expected_error);
    printf("result: %s\n", ok ? "PASS" : "FAIL");
    clear_lexer(lex);
    return (ok);
}

int	main(void)
{
    t_lex_result		*lex;
    t_test_case			cases[] = {
        {"basic word", "cat", 1, 0},
        {"two words", "cat ls", 2, 0},
        {"pipe", "cat|grep hi", 4, 0},
        {"redir out", "cat > file.txt", 3, 0},
        {"append", "cat >> file.txt", 3, 0},
        {"heredoc", "cat << EOF", 3, 0},
        {"double quoted word", "echo \"hello world\"", 2, 0},
        {"empty single quotes", "echo ''", 2, 0},
        {"mixed quotes", "echo '\"'$?'\"'", 2, 0},
        {"unterminated quote", "echo \"unterminated", 1, 2},
        {"Both expand", "echo \"'$HOME' $HOME\" ", 2, 0},
        {"Only HOME expands Q_DOUBLE | Q_SINGLE", "echo \"$HOME\" '/$USER'", 3 ,0},
        {"should expand Q_NONE", "echo pre\"$HOME\"post", 2, 0},
        {"Shouldn't expand. Q_NONE", "echo '\"$HOME\"'", 2, 0},
        {"Only HOME expands Q_NONE", "echo a\"$HOME\"b'$USER'", 2, 0},
    };
    int				pass;
    int				total;
    int				i;

    lex = init_lexer();
    if (!lex)
        return (1);
    total = (int)(sizeof(cases) / sizeof(cases[0]));
    pass = 0;
    i = 0;
    while (i < total)
    {
        pass += run_case(lex, &cases[i]);
        i++;
    }
    printf("\nSummary: %d/%d passed\n", pass, total);
    free(lex);
    return (0);
}
