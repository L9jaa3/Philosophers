/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:31:45 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/31 20:53:57 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_global	*check_cases(int ac, char **av, t_global *args)
{
	if (ac == 6)
		args->must_eat = ft_atol(av[5]);
	else
		args->must_eat = -1;
	if (args->philosophers_nb <= 0 || args->time_to_die <= 0
		|| args->time_to_eat <= 0 || args->time_to_sleep <= 0
		|| (ac == 6 && args->must_eat <= 0))
	{
		ft_error("Error: Invalid argument values.");
		free(args);
		return (NULL);
	}
	if (ft_atol(av[1]) < 1 || ft_atol(av[1]) > 200)
	{
		ft_error("philosophers arg is too low or more than required [1 - 200]");
		return (NULL);
	}
	if (args->time_to_die < 60 || args->time_to_eat < 60
		|| args->time_to_sleep < 60)
	{
		ft_error("Use timestamps greater than 60ms for realistic simulation");
		free(args);
		return (NULL);
	}
	return (args);
}

t_global	*ft_store_args(int ac, char **av)
{
	t_global	*args;

	args = ft_malloc(sizeof(t_global));
	if (!args)
		return (NULL);
	memset(args, 0, sizeof(t_global));
	args->philosophers_nb = ft_atol(av[1]);
	args->time_to_die = ft_atol(av[2]);
	args->time_to_eat = ft_atol(av[3]);
	args->time_to_sleep = ft_atol(av[4]);
	args = check_cases(ac, av, args);
	return (args);
}

void	start_working_mfs(t_global *args)
{
	int	i;

	args->simulation_end = false;
	mutex_init(args);
	philo_spawner(args);
	monitoring(args);
	i = 0;
	while (i < args->philosophers_nb)
	{
		ft_thread_error_handler
				(&args->philosophers[i].thread, NULL, NULL, JOIN_THREAD);
		i++;
	}
	destroy_mutex(args);
	free(args);
}

int	main(int ac, char **av)
{
	t_global	*args;

	args = NULL;
	if (ac < 5 || ac > 6)
	{
		ft_error("Error: incorrect number of arguments");
		return (1);
	}
	args = ft_store_args(ac, av);
	if (!args)
		return (1);
	start_working_mfs(args);
	return (0);
}
