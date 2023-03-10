/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asousa-n <asousa-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 09:44:34 by asousa-n          #+#    #+#             */
/*   Updated: 2022/12/21 09:45:09 by asousa-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <stdio.h>

void	send_msg(pid_t pid, char *msg)
{
	unsigned char	c;
	int				i;

	while (*msg)
	{
		i = 8;
		c = *msg;
		while (i--)
		{
			if (c >> i & 1)
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			usleep(300);
		}
		msg++;
	}
	i = 8;
	while (i--)
	{
		kill(pid, SIGUSR2);
		usleep(300);
	}
}

void	handler_signals(int signal)
{
	static int	received = 0;

	if (signal == SIGUSR2)
		++received;
	else
	{
		ft_printf("Mensagem de %d, caracteres recebida", received);
		ft_putchar_fd('\n', 1);
		exit(0);
	}
}

void	config_signals(void)
{
	struct sigaction	sigact;

	sigact.sa_handler = &handler_signals;
	sigact.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sigact, 0);
	sigaction(SIGUSR2, &sigact, 0);
}

int	main(int argc, char **argv)
{
	pid_t		pid;
	char		*msg;

	if (argc != 3)
	{
		ft_printf("invalid arguments");
		return (1);
	}
	pid = ft_atoi(argv[1]);
	msg = argv[2];
	config_signals();
	send_msg(pid, msg);
	while (1)
		pause();
	return (0);
}
