/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:50:48 by ebellon           #+#    #+#             */
/*   Updated: 2022/01/31 17:28:34 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_routine(void *const arg)
{
	t_philo	*self;

	self = (t_philo *)arg;
	while (!self->satisfied && self->table->running)
	{
		if (philo_eat(self) == EXIT_FAILURE)
			return (NULL);
		if (!self->satisfied && philo_sleep(self) == EXIT_FAILURE)
			return (NULL);
		if (self->table->rules.b_max_meal
			&& self->n_meal >= self->table->rules.max_meal)
			return (NULL);
	}
	return (arg);
}

unsigned char	create_philo(t_philo *philo, t_table *table)
{
	static uint64_t	philo_id = 0;

	philo_id++;
	philo->id = philo_id;
	philo->t_birth = get_time();
	philo->t_last_eat = philo->t_birth;
	philo->n_meal = 0;
	philo->satisfied = 0;
	philo->table = table;
	philo->lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (philo->lock && pthread_mutex_init(philo->lock, NULL) != 0)
		return (EXIT_FAILURE);
	if ((pthread_create(&philo->thread, NULL, philo_routine, philo)) != 0)
		return (EXIT_FAILURE);
	if ((pthread_detach(philo->thread)) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
