/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:50:41 by ebellon           #+#    #+#             */
/*   Updated: 2022/02/03 14:40:02 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	philo_talk(t_philo *philo, int action_id)
{
	uint64_t	time;
	static char	*action[5] = {
		"has taken a fork",
		"is eating",
		"is sleeping",
		"is thinking",
		"died"
	};

	if (pthread_mutex_lock(&philo->table->speak_lock) != 0)
		return (EXIT_FAILURE);
	time = get_time();
	if (!philo->table->running)
	{
		if (pthread_mutex_unlock(&philo->table->speak_lock) != 0)
			return (EXIT_FAILURE);
		return (time);
	}
	printf ("\033[1;32m%6llu\033[0m %2llu %s\n",
		time - philo->table->start_time, philo->id, action[action_id]);
	if (action_id != 5)
		if (pthread_mutex_unlock(&philo->table->speak_lock) != 0)
			return (EXIT_FAILURE);
	return (time);
}

unsigned char	philo_eat(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->lock) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(philo->forks[0]) != 0)
		return (EXIT_FAILURE);
	if (philo_talk(philo, A_FORK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(philo->forks[1]) != 0)
		return (EXIT_FAILURE);
	if (philo_talk(philo, A_FORK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	philo->t_last_eat = philo_talk(philo, A_EAT);
	if (philo->table->rules.b_max_meal)
		philo->n_meal++;
	if (!philo->satisfied && philo->table->rules.b_max_meal
		&& philo->n_meal == philo->table->rules.max_meal)
		philo->satisfied = 1;
	sleep_until(philo->t_last_eat + philo->table->rules.t_eat);
	if (pthread_mutex_unlock(philo->forks[0]) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_unlock(philo->forks[1]) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_unlock(&philo->lock) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

unsigned char	philo_sleep(t_philo *philo)
{
	sleep_until(philo_talk(philo, A_SLEEP) + philo->table->rules.t_sleep);
	philo_talk(philo, A_THINK);
	return (EXIT_SUCCESS);
}

int	all_satisfied(t_table *table)
{
	uint64_t		satisfied;
	unsigned char	i;

	i = 0;
	satisfied = 0;
	while (i < table->n_philo)
	{
		satisfied += table->philos[i].satisfied;
		if (table->rules.b_max_meal > 0 && satisfied == table->n_philo)
			return (1);
		if (get_time_since(table->philos[i].t_last_eat) >= table->rules.t_die)
		{
			philo_talk(table->philos + i, A_DIE);
			table->running = 0;
			pthread_mutex_unlock(&table->speak_lock);
			return (1);
		}
		i++;
	}
	if (table->rules.b_max_meal > 0 && satisfied == table->n_philo)
		return (1);
	return (0);
}

int	free_table(t_table *table)
{
	int	time;

	time = get_time();
	sleep_until(time + table->rules.t_eat + 2 * table->rules.t_sleep);
	free(table->philos);
	free(table->forks);
	free(table);
	return (EXIT_SUCCESS);
}
