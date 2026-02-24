/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 00:00:00 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: signal handling
// - Ctrl+C (SIGINT) -> new prompt
// - Ctrl+D (EOF) -> exit shell
// - Ctrl+\ (SIGQUIT) -> do nothing in interactive mode
// - different behavior in child processes
