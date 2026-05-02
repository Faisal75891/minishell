/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:38:58 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 23:21:41 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../include/minishell.h"

// first != 0 -> first char of variable name, else subsequent char
int	ms_is_var_char(int c, int first)
{
	if (first)
		return (ft_isalpha((unsigned char)c) || c == '_');
	return (ft_isalnum((unsigned char)c) || c == '_');
}

int	set_mode(const char	*s, int *i, int mode)
{
	if (mode == 0 && s[*i] == '\'')
	{
		(*i)++;
		return (1);
	}
	if (mode == 1 && s[*i] == '\'')
	{
		(*i)++;
		return (0);
	}
	if (mode == 0 && s[*i] == '"')
	{
		(*i)++;
		return (2);
	}
	if (mode == 2 && s[*i] == '"')
	{
		(*i)++;
		return (0);
	}
	return (mode);
}

char	*append_char(char *res, const char *s, int *i, t_shell *shell)
{
	char	*to_add;

	if (s[*i] == '$')
	{
		to_add = handle_dollar_seq(s, i, shell);
		res = ms_strappend_free(res, to_add);
	}
	else
	{
		res = ms_strappend_char(res, s[*i]);
		(*i)++;
	}
	return (res);
}

void	handle_error(t_lex_result *lexer)
{
	if (!lexer || lexer->error == 0)
		return ;
	if (lexer->error == 2)
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
	else if (lexer->error == 1)
		ft_putendl_fd("minishell: allocation error", 2);
	else if (lexer->error == 3)
	{
		if (!lexer->unexpected_token)
			lexer->unexpected_token = "newline";
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(lexer->unexpected_token, 2);
		ft_putendl_fd("'", 2);
	}
}

// i didn't know where to put this lol
int	split_len(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}
