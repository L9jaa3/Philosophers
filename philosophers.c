/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:31:39 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/10 22:21:21 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if(philo->philo_id % 2) // sleep odd philos for protect our routine from deadlock
		ft_usleep(philo->args->time_to_eat / 2);
	
	while(!is_simulation_ended(philo->args)) // Check simulation_end before each iteration
	{
		if (is_simulation_ended(philo->args)) // Check simulation_end before locking mutexes
			break;
		
		ft_mutex_error_handler(&philo->args->forks[philo->right_fork], LOCK); // catch the right fork and lock it
		
		if (is_simulation_ended(philo->args))
		{
			ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
			break;
		}
		dispaly_action("has taken a fork", philo);
		// Handle case with only one philosopher
		if (philo->args->philosophers_nb == 1)
		{
			dispaly_action("is thinking", philo);
			ft_usleep(philo->args->time_to_die);
			ft_mutex_error_handler(&philo->args->forks[philo->right_fork], UNLOCK);
			return (NULL);
		}
		
		//check simulation_end again before locking more mutexes
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
		
		// check simulation_end before sleeping
		if (is_simulation_ended(philo->args))
			break;

		dispaly_action("is sleeping", philo);
		ft_usleep(philo->args->time_to_sleep);
		
		// check simulation_end before thinking
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
	args->start = getting_curr_time(); // millisecond
	while(i < args->philosophers_nb)
	{
		philo = &args->philosophers[i];
		philo->philo_id = i;
		philo->right_fork = i;
		philo->left_fork = (i + 1) % args->philosophers_nb;
		pthread_mutex_lock(&args->meal); // lock to set
		philo->meals_nb = 0;
		philo->last_meal = getting_curr_time();
		pthread_mutex_unlock(&args->meal); // unlock after set
		philo->args = args;
		ft_thread_error_handler(&philo->thread, philo_routine, philo, CREATE_THREAD);
		i++;
	}
}

int		all_philos_eating(t_global *args)
{
	int philo_n;
	int	i;

	philo_n = 0;
	i = 0;
	while(i < args->philosophers_nb)
	{
		ft_mutex_error_handler(&args->meal, LOCK);
		if(args->philosophers[i].meals_nb >= args->must_eat)
			philo_n++;
		ft_mutex_error_handler(&args->meal, UNLOCK);
		i++;
	}
	return (philo_n == args->philosophers_nb);
}

void	monitoring(t_global *args)
{
	int				i;
	time_t	last_meal;
	
	while(!is_simulation_ended(args))
	{
		if(args->must_eat && all_philos_eating(args))
		{
			end_simulation(args);
			return ;
		}
		i = 0;
		while (i < args->philosophers_nb)
		{
			ft_mutex_error_handler(&args->meal, LOCK);
			last_meal = args->philosophers[i].last_meal;
			ft_mutex_error_handler(&args->meal, UNLOCK);
			if (getting_curr_time() >= last_meal + args->time_to_die)
			{
				ft_mutex_error_handler(&args->print, LOCK);
				printf("%ld  %d died\n", getting_curr_time() - args->start, i + 1);
				ft_mutex_error_handler(&args->print, UNLOCK);
				end_simulation(args);
				return ;
			}
			i++;
		}
		ft_usleep(1); // Prevent CPU thrashing in the monitoring loop
	}
}