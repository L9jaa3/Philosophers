#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

typedef struct s_needed
{
    int     philosophers;
    unsigned    int     time_to_die;
    unsigned    int     time_to_eat;
    unsigned    int     time_to_sleep;
    int     must_eat;
} t_needed;

int     atoi(const char *str);
void    ft_error(const char *str);