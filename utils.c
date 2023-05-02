/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:11:13 by mgraefen          #+#    #+#             */
/*   Updated: 2023/04/27 11:47:45 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *str)
{
	int			i;
	int			sign;
	long		out;

	i = 0;
	out = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = (sign * -1);
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (str[i] >= '0' && str[i] <= '9')
	{
		while (str[i] >= '0' && str[i] <= '9')
			out = ((out * 10) + (str[i++] - '0'));
		return (out * sign);
	}
	return (0);
}

int	is_valid_num(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		return (0);
	if (arg[i] == '+')
		i++;
	if (!arg[i] || arg[i] == '0')
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_input(int argc, char **argv, t_data *data)
{
	int	i;

	i = 1;
	if (argc > 6 || argc < 5)
		return (printf("No valid input!\n"), 0);
	while (argv[i])
		if (!is_valid_num(argv[i++]))
			return (printf("No valid input!\n"), 0);
	data->number_of_philosophers = ft_atol(argv[1]);
	data->time_to_die = (unsigned long) ft_atol(argv[2]);
	data->time_to_eat = (unsigned long) ft_atol(argv[3]);
	data->time_to_sleep = (unsigned long) ft_atol(argv[4]);
	if (argv[5])
		data->number_of_times_each_philosopher_must_eat = ft_atol(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = 0;
	return (1);
}

void	ms_sleep(unsigned long wait, t_data *data)
{
	unsigned long	start;

	start = get_timestamp(data);
	while (get_timestamp(data) < start + wait)
		usleep(100);
}

unsigned long	get_timestamp(t_data *data)
{
	struct timeval	time;
	unsigned long	timestamp;

	gettimeofday(&time, NULL);
	timestamp = (time.tv_sec - data->start_time.tv_sec) * 1000
		+ (time.tv_usec - data->start_time.tv_usec) / 1000;
	return (timestamp);
}
