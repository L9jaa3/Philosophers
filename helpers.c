/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:44:47 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/09 15:45:47 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	is_simulation_ended(t_global *args)
{
	bool ended;
	
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