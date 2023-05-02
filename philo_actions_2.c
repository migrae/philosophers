/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:33:09 by mgraefen          #+#    #+#             */
/*   Updated: 2023/04/27 11:46:38 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	left_p_eat(int *go, t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->exit_check);
	if (!go || data->exit)
		return (*go = 0, pthread_mutex_unlock(&data->exit_check), (void) NULL);
	pthread_mutex_unlock(&data->exit_check);
	pthread_mutex_lock(philo->left_fork);
	print_state(take_fork, go, philo, data);
	pthread_mutex_lock(&philo->my_fork);
	print_state(take_fork, go, philo, data);
	pthread_mutex_lock(&philo->last_eaten_check);
	philo->last_eaten = get_timestamp(data);
	print_state(eating, go, philo, data);
	philo->eat_counter++;
	if (data->number_of_times_each_philosopher_must_eat
		&& philo->eat_counter
		>= data->number_of_times_each_philosopher_must_eat)
			philo->finished_eating = 1;
	pthread_mutex_unlock(&philo->last_eaten_check);
	ms_sleep(data->time_to_eat, data);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&philo->my_fork);
}

void	one_philo(int *go, t_philo *philo, t_data *data)
{
	while (go)
	{
		pthread_mutex_lock(&data->exit_check);
		if (!go || data->exit)
			return (*go = 0, pthread_mutex_unlock(&data->exit_check),
				(void) NULL);
		pthread_mutex_unlock(&data->exit_check);
		pthread_mutex_lock(&philo->my_fork);
		print_state(take_fork, go, philo, data);
		ms_sleep(data->time_to_die + 5, data);
		pthread_mutex_unlock(&philo->my_fork);
	}
}
