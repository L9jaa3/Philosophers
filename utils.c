#include "philo.h"

void    ft_error(const char *str)
{
    int i;

    i = 0;
    while(str[i])
        write(2, &str[i], 1);
    exit(1);
}
int	ft_atoi(const char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > INT_MAX / 10 || (result
				== INT_MAX / 10 && str[i] - '0' > INT_MAX % 10))
			return (0);
		result = (result * 10) + (str[i] - '0');
		if ((str[i + 1] < '1' || str[i + 1] > '9') && str[i + 1] != '\0')
			return (0);
		i++;
	}
    if(str[i] != '\0')
        return (-1);
    return (result);
}
