
#include "include/minishell.h"

// works.
int main(void)
{
    t_lex_result *lex = init_lexer();

    printf("==================Test1=======================\n");
    enumerate_lexer("cat << EOF > file.txt", lex);

    t_token *current = lex->head;
    while(current)
    {
        printf("%d. ", lex->count);
        printf("Word : [%s]\n", current->word);
        printf("Type : %d\n", current->type);
        printf("Quote: %d\n", current->quote);
        printf("==========\n");
        current = current->next;
    }
    clear_lexer(lex);
    printf("==================Test2=======================\n");
    enumerate_lexer("cat << \"EOF\"", lex);

    current = lex->head;
    while(current)
    {
        printf("%d. ", lex->count);
        printf("Word : [%s]\n", current->word);
        printf("Type : %d\n", current->type);
        printf("Quote: %d\n", current->quote);
        printf("==========\n");
        current = current->next;
    }
    clear_lexer(lex);
    printf("==================Test3=======================\n");
    enumerate_lexer("cat | grep \"hello\"", lex);

    current = lex->head;
    while(current)
    {
        printf("%d. ", lex->count);
        printf("Word : [%s]\n", current->word);
        printf("Type : %d\n", current->type);
        printf("Quote: %d\n", current->quote);
        printf("==========\n");
        current = current->next;
    }
    clear_lexer(lex);
}
