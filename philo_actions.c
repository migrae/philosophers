/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 09:06:14 by mgraefen          #+#    #+#             */
/*   Updated: 2023/05/02 11:07:51 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_state state, int *go, t_philo *philo, t_data *data)
{
	char	*str;

	str = NULL;
	pthread_mutex_lock(&data->exit_check);
	if (data->exit)
		return (*go = 0, pthread_mutex_unlock(&data->exit_check), (void) NULL);
	pthread_mutex_unlock(&data->exit_check);
	if (state == take_fork)
		str = "has taken a fork";
	if (state == eating)
		str = "is eating";
	if (state == sleeping)
		str = "is sleeping";
	if (state == thinking)
		str = "is thinking";
	if (state == dead)
		str = "died";
	if (state == finish)
		str = "everyone had enough";
	pthread_mutex_lock(&data->print_lock);
	printf("%lu: %d %s\n", get_timestamp(data), philo->num, str);
	pthread_mutex_unlock(&data->print_lock);
}

void	eat_sleep_repeat(int *go, t_philo *philo, t_data *data)
{
	if (data->number_of_philosophers == 1)
		return (one_philo(go, philo, data), (void) NULL);
	if (philo->num % 2 == 0)
		left_p_eat(go, philo, data);
	else
		p_eat(go, philo, data);
	p_sleep(go, philo, data);
	p_think(go, philo, data);
	if (philo->num % 2 != 0)
		ms_sleep(1, data);
}

void	p_eat(int *go, t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->exit_check);
	if (data->exit)
		return (*go = 0, pthread_mutex_unlock(&data->exit_check), (void) NULL);
	pthread_mutex_unlock(&data->exit_check);
	pthread_mutex_lock(&philo->my_fork);
	print_state(take_fork, go, philo, data);
	pthread_mutex_lock(philo->left_fork);
	print_state(take_fork, go, philo, data);
	pthread_mutex_lock(&philo->last_eaten_check);
	philo->last_eaten = get_timestamp(data);
	print_state(eating, go, philo, data);
	philo->eat_counter++;
	if (data->number_of_times_each_philosopher_must_eat
		&& philo->eat_counter
		== data->number_of_times_each_philosopher_must_eat)
			philo->finished_eating = 1;
	pthread_mutex_unlock(&philo->last_eaten_check);
	ms_sleep(data->time_to_eat, data);
	pthread_mutex_unlock(&philo->my_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	p_sleep(int *go, t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->exit_check);
	if (data->exit)
		return (*go = 0, pthread_mutex_unlock(&data->exit_check), (void) NULL);
	pthread_mutex_unlock(&data->exit_check);
	print_state(sleeping, go, philo, data);
	ms_sleep(data->time_to_sleep, data);
}

void	p_think(int *go, t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->exit_check);
	if (data->exit)
		return (*go = 0, pthread_mutex_unlock(&data->exit_check), (void) NULL);
	pthread_mutex_unlock(&data->exit_check);
	print_state(thinking, go, philo, data);
}
