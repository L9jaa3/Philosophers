/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:32:35 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/29 14:38:29 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_error(const char *str)
{
	printf ("%s\n", str);
	return (1);
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
	time_t	elapsed;

	start = getting_curr_time();
	while (1)
	{
		elapsed = getting_curr_time() - start;
		if (elapsed >= time)
			break;
		
		// Use different sleep strategies based on remaining time
		if (time - elapsed > 10)
			usleep(1000); // Sleep 1ms if more than 10ms remaining
		else
			usleep(100);  // Sleep 100μs for precision at the end
	}
}

static long	handle_overflow(int sign, unsigned long long n, int count)
{
	if (((n > LLONG_MAX || count > 19) && sign == -1)
		|| ((n > LLONG_MAX || count > 19) && sign == 1))
		return (0);
	return ((long)(n * sign));
}

long	ft_atol(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	result;
	int					counter;

	i = 0;
	sign = 1;
	result = 0;
	counter = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	if (str[i] < '0' || str[i] > '9')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		counter++;
		i++;
	}
	if (str[i] != '\0')
		return (0);
	return (handle_overflow(sign, result, counter));
}
