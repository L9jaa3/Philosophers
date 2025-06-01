/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:36:26 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/29 21:08:36 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_malloc(size_t size)
{
	void	*expected;

	expected = malloc (size);
	if (expected == NULL)
		ft_error("unexpected error with malloc");
	return (expected);
}

static void	handle_mutex_status_error(int status, t_mutex_codes opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
		ft_error("Mutex error: Invalid mutex (LOCK/UNLOCK)");
	else if (status == EINVAL && opcode == INIT)
		ft_error("Mutex error: Invalid attributes in INIT");
	else if (status == EDEADLK)
		ft_error("Mutex error: Deadlock detected");
	else if (status == EPERM)
		ft_error("Mutex error: Unlock attempted by wrong thread");
	else if (status == ENOMEM)
		ft_error("Mutex error: Not enough memory to init mutex");
	else if (status == EBUSY)
		ft_error("Mutex error: Destroy attempt on locked mutex");
	else
		ft_error("Mutex error: Unknown error occurred");
}

void	ft_mutex_error_handler(pthread_mutex_t *mutex, t_mutex_codes opcode)
{
	int	status;

	status = 0;
	if (opcode == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (opcode == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (opcode == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		ft_error("Mutex error: Invalid operation code");
	handle_mutex_status_error(status, opcode);
}

static void	handle_thread_status_error(int status, t_thread_code opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL
		&& (opcode == JOIN_THREAD || opcode == DETACH_THREAD))
		ft_error("Thread error: Invalid or not joinable");
	else if (status == EDEADLK)
		ft_error("Thread error: Deadlock on join");
	else if (status == ESRCH)
		ft_error("Thread error: No thread found with that ID");
	else if (status == EPERM)
		ft_error("Thread error: Operation not permitted");
	else if (status == ENOMEM && opcode == CREATE_THREAD)
		ft_error("Thread error: Not enough memory to create");
	else
		ft_error("Thread error: Unknown error");
}

void	ft_thread_error_handler(pthread_t *thread,
	void *(*routine)(void *), void *arg, t_thread_code opcode)
{
	int	status;

	status = 0;
	if (opcode == CREATE_THREAD)
		status = pthread_create(thread, NULL, routine, arg);
	else if (opcode == JOIN_THREAD)
		status = pthread_join(*thread, NULL);
	else if (opcode == DETACH_THREAD)
		status = pthread_detach(*thread);
	else
		ft_error("Thread error: Invalid operation code");
	handle_thread_status_error(status, opcode);
}