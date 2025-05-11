/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:20:00 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/11 16:21:45 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	check_philosopher_death(t_philo *philo)
{
	time_t	current_time;
	time_t	time_since_last_meal;
	
	ft_mutex_error_handler(&philo->args->meal, LOCK);
	current_time = getting_curr_time();
	time_since_last_meal = current_time - philo->last_meal;
	
	if (time_since_last_meal > philo->args->time_to_die)
	{
		ft_mutex_error_handler(&philo->args->meal, UNLOCK);
		
		// Announce death and end simulation
		ft_mutex_error_handler(&philo->args->print, LOCK);
		printf("%ld %d died\n", getting_curr_time() - philo->args->start, 
			philo->philo_id + 1);
		ft_mutex_error_handler(&philo->args->print, UNLOCK);
		
		end_simulation(philo->args);
		return (true);
	}
	ft_mutex_error_handler(&philo->args->meal, UNLOCK);
	return (false);
}

static bool	check_all_philosophers_ate(t_global *args)
{
	int	i;
	int	full_philosophers;
	
	if (args->must_eat <= 0)
		return (false);
		
	full_philosophers = 0;
	i = 0;
	while (i < args->philosophers_nb)
	{
		ft_mutex_error_handler(&args->meal, LOCK);
		if (args->philosophers[i].it_is_full)
			full_philosophers++;
		ft_mutex_error_handler(&args->meal, UNLOCK);
		i++;
	}
	
	if (full_philosophers == args->philosophers_nb)
	{
		end_simulation(args);
		return (true);
	}
	return (false);
}

void	monitoring(t_global *args)
{
	int		i;
	bool	simulation_ended;
	
	simulation_ended = false;
	while (!simulation_ended)
	{
		i = 0;
		while (i < args->philosophers_nb && !simulation_ended)
		{
			if (check_philosopher_death(&args->philosophers[i]))
			{
				simulation_ended = true;
				break;
			}
			i++;
		}
		
		if (!simulation_ended && check_all_philosophers_ate(args))
			simulation_ended = true;
			
		usleep(1000); // Sleep 1ms to avoid high CPU usage
	}
}