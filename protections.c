/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:36:26 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/19 16:25:26 by ielouarr         ###   ########.fr       */
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

static void	handle_mutex_status_error(int status, t_mutex_codes	opcode)
{
	if (status == 0)
		return ;
	if ((status == EINVAL)
		&& (opcode == LOCK || opcode == UNLOCK))
		ft_error("Mutex error: Invalid mutex (LOCK/UNLOCK)");
	else if (status == EINVAL && opcode == INIT)
		ft_error("Mutex error: Invalid attributes in INIT");
	else if (status == EDEADLK)
		ft_error("Mutex error: Deadlock detected");
	else if (status == EPERM)
		ft_error("Mutex error: Unlock attempted> thread not owning the mutex");
	else if (status == ENOMEM)
		ft_error("Mutex error: Not enough memory to initialize mutex");
	else if (status == EBUSY)
		ft_error("Mutex error: Attempt to destroy a locked mutex");
	else
		ft_error("Mutex error: Unknown error occurred");
}

void	ft_mutex_error_handler(pthread_mutex_t *mutex, t_mutex_codes opcode)
{
	if (LOCK == opcode)
		handle_mutex_status_error(pthread_mutex_lock(mutex), opcode);
	else if (UNLOCK == opcode)
		handle_mutex_status_error(pthread_mutex_unlock(mutex), opcode);
	else if (INIT == opcode)
		handle_mutex_status_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		handle_mutex_status_error(pthread_mutex_destroy(mutex), opcode);
	else
		ft_error("Wrong opcode for mutex handle");
}

static void	handle_thread_status_error(int status, t_thread_code opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL
		&& (opcode == JOIN_THREAD || opcode == DETACH_THREAD))
		ft_error("Invalid thread or thread not joinable");
	else if (status == EDEADLK)
		ft_error("Deadlock detected on thread join");
	else if (status == ESRCH)
		ft_error("No thread found with that ID");
	else if (status == EPERM)
		ft_error("Thread cannot be joined or detached");
	else if (status == ENOMEM && opcode == CREATE_THREAD)
		ft_error("Not enough memory to create thread");
	else
		ft_error("Unknown thread error");
}

void	ft_thread_error_handler(pthread_t *thread, void *(*routine)(void *),
	void *arg, t_thread_code opcode)
{
	if (opcode == CREATE_THREAD)
		handle_thread_status_error(pthread_create
			(thread, NULL, routine, arg), opcode);
	else if (opcode == JOIN_THREAD)
		handle_thread_status_error(pthread_join(*thread, NULL), opcode);
	else if (opcode == DETACH_THREAD)
		handle_thread_status_error(pthread_detach(*thread), opcode);
	else
		ft_error("Invalid thread opcode");
}
