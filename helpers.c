/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:44:47 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/19 17:38:49 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	dispaly_action(char *action, t_philo *philo)
{
	t_global	*args;

	args = philo->args;
	if (is_simulation_ended(args))
		return ;
	ft_mutex_error_handler(&args->print, LOCK);
	if (!is_simulation_ended(args))
		printf("%ld %d %s\n", getting_curr_time() - args->start,
			philo->philo_id + 1, action);
	ft_mutex_error_handler(&args->print, UNLOCK);
}

bool	is_simulation_ended(t_global *args)
{
	bool	ended;

	ft_mutex_error_handler(&args->status, LOCK);
	ended = args->simulation_end;
	ft_mutex_error_handler(&args->status, UNLOCK);
	return (ended);
}

void	end_simulation(t_global *args)
{
	ft_mutex_error_handler(&args->status, LOCK);
	args->simulation_end = true;
	ft_mutex_error_handler(&args->status, UNLOCK);
}
