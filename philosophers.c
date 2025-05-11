/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:31:39 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/11 18:12:03 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if(philo->philo_id % 2) 
		ft_usleep(philo->args->time_to_eat / 2);
	
	while(!is_simulation_ended(philo->args))
	{
		if (is_simulation_ended(philo->args))
			break;
		
		ft_mutex_error_handler(&philo->args->forks[philo->right_fork], LOCK);
		
		if (is_simulation_ended(philo->args))
		{
			ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
			break;
		}
		dispaly_action("has taken a fork", philo);
		if (philo->args->philosophers_nb == 1)
		{
			ft_usleep(philo->args->time_to_die);
			ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
			return (NULL);
		}
		
		if (is_simulation_ended(philo->args))
		{
			ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
			break;
		}
		
		ft_mutex_error_handler(&philo->args->forks[philo->left_fork], LOCK);// catch the left fork and lock it
		if (is_simulation_ended(philo->args))
		{
			ft_mutex_error_handler(&philo->args->forks[philo->left_fork], UNLOCK);
			ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
			break;
		}
		dispaly_action("has taken a fork", philo);
		dispaly_action("is eating", philo);
		
		ft_mutex_error_handler(&philo->args->meal, LOCK);
		philo->last_meal = getting_curr_time();
		philo->meals_nb++;
		
		if (philo->args->must_eat > 0 && philo->meals_nb >= philo->args->must_eat)
			philo->it_is_full = true;
			
		ft_mutex_error_handler(&philo->args->meal, UNLOCK);
		
		ft_usleep(philo->args->time_to_eat);
		
		ft_mutex_error_handler(&philo->args->forks[philo->left_fork], UNLOCK);
		ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);

		if (is_simulation_ended(philo->args))
			break;

		dispaly_action("is sleeping", philo);
		ft_usleep(philo->args->time_to_sleep);
		
		if (is_simulation_ended(philo->args))
			break;
		dispaly_action("is thinking", philo);
	}
	return (NULL);
}
void	philo_spawner(t_global *args)
{
	int		i;
	t_philo	*philo;
	
	i = 0;
	args->start = getting_curr_time();
	while(i < args->philosophers_nb)
	{
		philo = &args->philosophers[i];
		philo->philo_id = i;
		philo->right_fork = i;
		philo->left_fork = (i + 1) % args->philosophers_nb;
		pthread_mutex_lock(&args->meal);
		philo->meals_nb = 0;
		philo->last_meal = getting_curr_time();
		pthread_mutex_unlock(&args->meal);
		philo->args = args;
		ft_thread_error_handler(&philo->thread, philo_routine, philo, CREATE_THREAD);
		i++;
	}
}