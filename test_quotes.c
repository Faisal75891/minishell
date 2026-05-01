#include <stdio.h>
#include <stdlib.h>


int ft_strlen(const char *s)
{
    int len;

    len = 0;
    while (s[len])
        len++;
    return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*string;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	string = malloc(len);
	if (!string)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		string[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		string[i + j] = s2[j];
		j++;
	}
	string[i + j] = '\0';
	return (string);
}

char	*ft_strdup(const char *s)
{
	char	*string;
	int		i;
	int		len;

	len = ft_strlen(s);
	string = malloc(len + 1);
	if (!string)
		return (NULL);
	i = 0;
	while (i < len)
	{
		string[i] = s[i];
		i++;
	}
	string[i] = '\0';
	return (string);
}


char	*ms_strappend_char(char *s, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(s, buf);
	free(s);
	return (tmp);
}

static int	is_quote(char c, int quote)
{
	if (c == '"' && quote != '\'')
	{
		if (quote == '"')
			return (0);
		return ('"');
	}
	if (c == '\'' && quote != '"')
	{
		if (quote == '\'')
			return (0);
		return ('\'');
	}
	return (quote);
}


int main(void)
{
    char    *word[] = {
		"Hey lo \\\" ",
		"echo \\\"unterminated",
		"echo 'unterminated",
		"echo \\\"abc'def",      // unmatched double quote
		"echo 'abc\\\"def",      // unmatched single quote
		"echo \\\"",             // only opening double quote
		"echo '",              // only opening single quote
		"echo \\\"a'b",          // missing closing double quote
		"echo 'a\\\"b",          // missing closing single quote
		"\ \\",
		"\ ",
		"\\",
		"'\\'",
		NULL
	};
	
	for (int j = 0; word[j]; j++)
	{
 	   	char    *buffer = ft_strdup("");
 	   	printf("word: [%s]\n", word[j]);
		int 	i = 0;
		int     new_quote = 0;
		int 	quote = 0;
		while (word[j][i])
		{
			int consumed = 1;

			if (word[j][i] == '\\' && quote != '\'')
			{
				if (word[j][i + 1])  // escape next char
				{
					buffer = ms_strappend_char(buffer, word[j][i + 1]);
					consumed = 2;
				}
				else
					buffer = ms_strappend_char(buffer, word[j][i]); // trailing backslash
			}
			else
			{
				int new_quote = is_quote(word[j][i], quote);
				if (new_quote != quote)
				{
					quote = new_quote;   // quote char is syntax, not output
				}
				else
				{
					buffer = ms_strappend_char(buffer, word[j][i]);
				}
			}
	
			i += consumed;
		}
		if (quote != 0)
			printf("Syntax error\n");
		printf("buffer: [%s]\n\n", buffer);
    	free(buffer);
	}
}