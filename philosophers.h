/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:31:31 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/19 16:32:05 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>

typedef struct s_philo
{
	int				philo_id;
	pthread_t		thread;
	long			meals_nb;
	bool			it_is_full;
	time_t			last_meal;
	int				right_fork;
	int				left_fork;
	struct s_global	*args;
}	t_philo;

typedef struct s_global
{
	long				philosophers_nb ;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	int					must_eat;
	time_t				start;
	pthread_mutex_t		meal;
	pthread_mutex_t		print;
	pthread_mutex_t		status;
	pthread_mutex_t		forks[200];
	t_philo				philosophers[200];
	bool				simulation_end;
}	t_global;

typedef enum e_mutex_codes
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_mutex_codes;

typedef enum e_thread_code
{
	CREATE_THREAD,
	JOIN_THREAD,
	DETACH_THREAD
}	t_thread_code;

long	ft_atol(const char *str);
int		ft_error(const char *str);
void	dispaly_action(char *action, t_philo *philo);
long	getting_curr_time(void);
void	ft_usleep(time_t time);
void	mutex_init(t_global *args);
void	destroy_mutex(t_global *args);
void	philo_spawner(t_global *args);
void	monitoring(t_global *args);

void	*ft_malloc(size_t size);
void	ft_mutex_error_handler(pthread_mutex_t *mutex, t_mutex_codes opcode);
void	ft_thread_error_handler(pthread_t *thread, void *(*routine)(void *),
			void *arg, t_thread_code opcode);
void	end_simulation(t_global *args);
bool	is_simulation_ended(t_global *args);

#endif