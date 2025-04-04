#include "philo.h"

void    ft_store_args(int ac, char **av)
{
    t_needed    args;

    if(ac < 5 || ac > 6)
        ft_error("Error : the args is less than needed");
    args.philosophers = ft_atoi(av[1]);
    args.time_to_die = ft_atoi(av[2]);
    args.time_to_eat = ft_atoi(av[3]);
    args.time_to_sleep = ft_atoi(av[4]);
    if(ac == 6)
        args.must_eat = ft_atoi(av[5]);
    else
        args.must_eat = -1;
    if (args.philosophers <= 0 || args.time_to_die <= 0 || args.time_to_eat <= 0 
                                    || args.time_to_sleep <= 0 || args.must_eat == -1)

        ft_error("Error: Invalid argument values.\n");
    
}
int main(int ac, char **av)
{
    
    ft_store_args(ac, av);
    
}