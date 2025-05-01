/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:31:45 by ielouarr          #+#    #+#             */
/*   Updated: 2025/05/01 13:33:52 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int    ft_store_args(int ac, char **av, t_global *args)
{
    args->philosophers_nb = ft_atol(av[1]);
    args->time_to_die = ft_atol(av[2]);
    args->time_to_eat = ft_atol(av[3]);
    args->time_to_sleep = ft_atol(av[4]);
    
    if (args->time_to_die < 60 || args->time_to_eat < 60 || args->time_to_sleep < 60)
        return (ft_error("Use timestamps greater than 60ms for realistic simulation"));
    
    if (ac == 6)
        args->must_eat = ft_atol(av[5]);
    else
        args->must_eat = -1;
        
    if (args->philosophers_nb <= 0 || args->time_to_die <= 0 || 
        args->time_to_eat <= 0 || args->time_to_sleep <= 0 || 
        (ac == 6 && args->must_eat <= 0))
        return (ft_error("Error: Invalid argument values.\n"));
    
    return (0);
}

int main(int ac, char **av)
{
    t_global    *args;

    if (ac < 5 || ac > 6)
        return (ft_error("Error: incorrect number of arguments")); 
    if (ft_atol(av[1]) < 1 || ft_atol(av[1]) > 200)
        return (ft_error("philosophers arg is too low or more than required [1 - 200]"));  
    args = ft_malloc(sizeof(t_global));
    if (!args)
        return (1);
    memset(args, 0, sizeof(t_global));
    if (ft_store_args(ac, av, args) != 0)
        return (1); 
    mutex_init(args);
    philo_spawner(args);
    monitoring(args);
    destroy_mutex(args);
    free(args);
    return (0);
}