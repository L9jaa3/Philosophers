/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:31:39 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/20 20:39:19 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	take_forks(t_philo *philo)
{
	ft_mutex_error_handler(&philo->args->forks[philo->right_fork], LOCK);
	if (is_simulation_ended(philo->args))
	{
		ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
		return (false);
	}
	dispaly_action("has taken a fork", philo);
	if (philo->args->philosophers_nb == 1)
	{
		ft_usleep(philo->args->time_to_die, philo->args);
		ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
		return (false);
	}
	ft_mutex_error_handler(&philo->args->forks[philo->left_fork], LOCK);
	if (is_simulation_ended(philo->args))
	{
		ft_mutex_error_handler(&philo->args->forks[philo->left_fork], UNLOCK);
		ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
		return (false);
	}
	dispaly_action("has taken a fork", philo);
	return (true);
}

static void	eat_sleep_think(t_philo *philo)
{
	dispaly_action("is eating", philo);
	ft_mutex_error_handler(&philo->args->meal, LOCK);
	philo->last_meal = getting_curr_time();
	philo->meals_nb++;
	if (philo->args->must_eat > 0
		&& philo->meals_nb >= philo->args->must_eat)
		philo->it_is_full = true;
	ft_mutex_error_handler(&philo->args->meal, UNLOCK);
	ft_usleep(philo->args->time_to_eat, philo->args);
	ft_mutex_error_handler(&philo->args->forks[philo->left_fork], UNLOCK);
	ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
	if (!is_simulation_ended(philo->args))
	{
		dispaly_action("is sleeping", philo);
		ft_usleep(philo->args->time_to_sleep, philo->args);
	}
	if (!is_simulation_ended(philo->args))
		dispaly_action("is thinking", philo);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->philo_id % 2)
		ft_usleep(philo->args->time_to_eat / 2, philo->args);
	while (!is_simulation_ended(philo->args))
	{
		if (!take_forks(philo))
			break ;
		eat_sleep_think(philo);
	}
	return (NULL);
}

void	philo_spawner(t_global *args)
{
	int		i;
	t_philo	*philo;

	i = 0;
	args->start = getting_curr_time();
	while (i < args->philosophers_nb)
	{
		philo = &args->philosophers[i];
		philo->philo_id = i;
		philo->right_fork = i;
		philo->left_fork = (i + 1) % args->philosophers_nb;
		philo->meals_nb = 0;
		philo->last_meal = getting_curr_time();
		philo->args = args;
		ft_thread_error_handler
				(&philo->thread, philo_routine, philo, CREATE_THREAD);
		i++;
	}
}
