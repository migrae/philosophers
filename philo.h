/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 09:20:37 by mgraefen          #+#    #+#             */
/*   Updated: 2023/04/28 09:40:15 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef enum e_state
{
	take_fork,
	eating,
	sleeping,
	thinking,
	dead,
	finish
}	t_state;

typedef struct s_philo
{
	int				num;
	int				eat_counter;
	int				finished_eating;
	unsigned long	last_eaten;
	pthread_t		philo_thread;
	pthread_mutex_t	my_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	last_eaten_check;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	t_philo			**philos;
	pthread_t		big_brother;
	pthread_mutex_t	exit_check;
	pthread_mutex_t	print_lock;
	int				exit;
	int				number_of_philosophers;
	unsigned long	time_to_die;
	int				time_to_eat ;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	struct timeval	start_time;
}	t_data;

//philo
t_data			*init_data(t_data *data);
int				init_philos(t_data *data);
int				release_the_philos(t_data *data);
void			*philo_routine(void *arg);

//philo_actions

void			print_state(t_state state, int *go, t_philo *philo,
					t_data *data);
void			eat_sleep_repeat(int *go, t_philo *philo, t_data *data);
void			p_eat(int *go, t_philo *philo, t_data *data);
void			p_sleep(int *go, t_philo *philo, t_data *data);
void			p_think(int *go, t_philo *philo, t_data *data);

//philo_actions_2
void			left_p_eat(int *go, t_philo *philo, t_data *data);
void			one_philo(int *go, t_philo *philo, t_data *data);

//big_brother
void			*big_brother_routine(void *arg);
void			kill_philo(t_data *data, t_philo *philo, int *go);

//utils
int				is_valid_num(char *arg);
long			ft_atol(const char *str);
int				check_input(int argc, char **argv, t_data *data);
void			ms_sleep(unsigned long wait, t_data *data);
unsigned long	get_timestamp(t_data *data);

#endif
