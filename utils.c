/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:32:35 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/11 18:29:08 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	dispaly_action(char *action, t_philo *philo)
{
	t_global	*args;

	args = philo->args;
	if(is_simulation_ended(args))
		return ;
	
	ft_mutex_error_handler(&args->print, LOCK);
	if(!is_simulation_ended(args))
		printf("%ld %d %s\n", getting_curr_time() - args->start, philo->philo_id + 1, action);
	ft_mutex_error_handler(&args->print, UNLOCK);
}

int    ft_error(const char *str)
{
	printf("%s\n", str);
	return(1);
}

long	getting_curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long)(time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(time_t time)
{
	time_t	start;

	start = getting_curr_time();
	if (time < 10)
	{
		while (getting_curr_time() < start + time)
			usleep(100);
	}
	else
	{
		while (getting_curr_time() < start + time)
			usleep(200);
	}
}

long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	return ((long)result * sign);
}