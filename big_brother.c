/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_brother.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:16:31 by mgraefen          #+#    #+#             */
/*   Updated: 2023/05/02 08:23:08 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	kill_philo(t_data *data, t_philo *philo, int *go)
{
	(void) philo;
	pthread_mutex_lock(&data->exit_check);
	data->exit = 1;
	pthread_mutex_unlock(&data->exit_check);
	pthread_mutex_lock(&data->print_lock);
	printf("%lu: %d died\n", get_timestamp(data), philo->num);
	pthread_mutex_unlock(&data->print_lock);
	*go = 0;
}

void	enough_already(t_data *data, t_philo *philo, int *go)
{
	(void) philo;
	pthread_mutex_lock(&data->exit_check);
	data->exit = 1;
	pthread_mutex_unlock(&data->exit_check);
	*go = 0;
}

int	observe(t_data *data, t_philo *philo, int *go, int *total_eat_counter)
{
	unsigned long	timestamp;

	pthread_mutex_lock(&philo->last_eaten_check);
	timestamp = get_timestamp(data);
	if (timestamp - philo->last_eaten > data->time_to_die)
	{
		kill_philo(data, philo, go);
		pthread_mutex_unlock(&philo->last_eaten_check);
		return (0);
	}
	*total_eat_counter += philo->finished_eating;
	if (data->number_of_times_each_philosopher_must_eat != 0
		&& *total_eat_counter == data->number_of_philosophers)
	{
		enough_already(data, philo, go);
		pthread_mutex_unlock(&philo->last_eaten_check);
		return (0);
	}
	pthread_mutex_unlock(&philo->last_eaten_check);
	return (1);
}

void	*big_brother_routine(void *arg)
{
	int				i;
	int				go;
	int				total_eat_counter;
	t_data			*data;

	i = 0;
	go = 1;
	data = (t_data *)arg;
	while (go)
	{
		i = 0;
		total_eat_counter = 0;
		while (i < data->number_of_philosophers && go)
		{
			if (!observe(data, data->philos[i], &go, &total_eat_counter))
				break ;
			i++;
		}
	}
	return (NULL);
}
