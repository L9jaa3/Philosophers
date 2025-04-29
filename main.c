/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielouarr <ielouarr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:31:45 by ielouarr          #+#    #+#             */
/*   Updated: 2025/04/29 14:47:27 by ielouarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int    ft_store_args(int ac, char **av, t_global *args)
{

    if(ac < 5 || ac > 6)
        ft_error("Error : the args is less than needed");
    if(av[1] < 1 || av[1] > 200)
        ft_error("philosophers arg is too low or more than required [1 - 200])");
    args->philosophers_nb = ft_atol(av[1]);
    args->time_to_die = ft_atol(av[2]) * 1e3; // from second to millisecond
    args->time_to_eat = ft_atol(av[3]) * 1e3;
    args->time_to_sleep = ft_atol(av[4]) * 1e3;
    if(args->time_to_die < 6e4 || args->time_to_eat < 6e4 || args->time_to_sleep < 6e4) //the args that content time , need be more than 60ms because some time between thread and thread take a while 15ms or 10ms , and have a realistic simulation
        ft_error("Use timestamps greater than 60ms for realistic simulation");
    if(ac == 6)
        args->must_eat = ft_atoi(av[5]);
    else
        args->must_eat = -1;
    if (args->philosophers <= 0 || args->time_to_die <= 0 || args->time_to_eat <= 0 
                                    || args->time_to_sleep <= 0 || args->must_eat == -1)
        ft_error("Error: Invalid argument values.\n");
    
}
int main(int ac, char **av)
{
    t_global    *args;
    if(ft_store_args(ac, av, args) == -1)
        return (1);
    mutex_init(args);
    philo_spawner(args);
    monitoring(args);
    destroy_mutex(args);
    return (0);
}