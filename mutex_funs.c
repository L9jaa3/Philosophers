/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_funs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:52 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/09 15:44:00 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mutex_init(t_global *args)
{
	int	i;

	ft_mutex_error_handler(&args->meal, INIT);
	ft_mutex_error_handler(&args->print, INIT);
	ft_mutex_error_handler(&args->status, INIT);
	i = 0;
	while (i < args->philosophers_nb)
	{
		ft_mutex_error_handler(&args->forks[i], INIT);
		i++;
	}
}

void	destroy_mutex(t_global *args)
{
	int	i;

	i = 0;
	while (i < args->philosophers_nb)
	{
		ft_mutex_error_handler(&args->forks[i], DESTROY);
		i++;
	}
	ft_mutex_error_handler(&args->print, DESTROY);
	ft_mutex_error_handler(&args->meal, DESTROY);
	ft_mutex_error_handler(&args->status, DESTROY);
}