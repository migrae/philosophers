/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 09:20:30 by mgraefen          #+#    #+#             */
/*   Updated: 2023/05/02 11:12:06 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo *) * data->number_of_philosophers);
	if (!data->philos)
		return (0);
	while (i < data->number_of_philosophers)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		if (!data->philos[i])
			return (0);
		pthread_mutex_init(&data->philos[i]->my_fork, NULL);
		pthread_mutex_init(&data->philos[i]->last_eaten_check, NULL);
		if (i != 0)
			data->philos[i]->left_fork = &data->philos[i - 1]->my_fork;
		data->philos[i]->num = i + 1;
		data->philos[i]->eat_counter = 0;
		data->philos[i]->finished_eating = 0;
		data->philos[i]->last_eaten = 0;
		i++;
	}
	data->philos[0]->left_fork = &data->philos[i - 1]->my_fork;
	return (1);
}

int	release_the_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_join(data->philos[i]->philo_thread, NULL))
			return (1);
		i++;
	}
	i = 0;
	if (pthread_join(data->big_brother, NULL))
		return (1);
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&data->philos[i]->my_fork);
		pthread_mutex_destroy(&data->philos[i]->last_eaten_check);
		free(data->philos[i]);
		i++;
	}
	pthread_mutex_destroy(&data->exit_check);
	pthread_mutex_destroy(&data->print_lock);
	free(data->philos);
	free(data);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		go;

	philo = (t_philo *)arg;
	data = philo->data;
	go = 1;
	pthread_mutex_lock(&philo->last_eaten_check);
	philo->last_eaten = get_timestamp(data);
	pthread_mutex_unlock(&philo->last_eaten_check);
	if (philo->num % 2 == 0)
		ms_sleep(1, data);
	if (philo->num % 3 == 0)
		ms_sleep(4, data);
	while (go)
		eat_sleep_repeat(&go, philo, data);
	return (NULL);
}

t_data	*init_data(t_data *data)
{
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->exit = 0;
	pthread_mutex_init(&data->exit_check, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	gettimeofday(&data->start_time, NULL);
	return (data);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	i = 0;
	data = NULL;
	data = init_data(data);
	if (!data)
		return (1);
	if (!check_input(argc, argv, data))
		return (free(data), 1);
	if (!init_philos(data))
		return (1);
	while (i < data->number_of_philosophers)
	{
		data->philos[i]->data = data;
		if (pthread_create(&data->philos[i]->philo_thread, NULL, &philo_routine,
				data->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&data->big_brother, NULL, &big_brother_routine, data))
		return (1);
	if (!release_the_philos(data))
		return (1);
	return (0);
}
