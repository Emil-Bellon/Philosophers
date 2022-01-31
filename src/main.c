/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:50:45 by ebellon           #+#    #+#             */
/*   Updated: 2022/01/31 17:32:21 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex(uint64_t n_forks, t_table *table)
{
	uint64_t	i;

	table->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * n_forks);
	if (!table->forks)
		return (EXIT_FAILURE);
	i = 0;
	while (i < n_forks)
		if (pthread_mutex_init(table->forks + i++, NULL) != 0)
			return (EXIT_FAILURE);
	if (pthread_mutex_init(&table->speak_lock, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	init_philo(uint64_t n_philo, t_table *table)
{
	uint64_t	i;

	i = 0;
	while (i < n_philo)
	{
		table->philos[i].forks[0] = table->forks + i;
		if (i < n_philo - 1)
			table->philos[i].forks[1] = table->forks + i + 1;
		else
			table->philos[i].forks[1] = table->forks;
		create_philo(table->philos + i, table);
		i++;
		usleep(50);
	}
}

static int	init_table(t_table_rules rules, uint64_t n_philo)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table || n_philo <= 1)
		return (EXIT_FAILURE);
	table->rules = rules;
	table->philos = malloc(sizeof(t_philo) * n_philo);
	if (!table->philos)
		return (EXIT_FAILURE);
	table->n_philo = n_philo;
	init_mutex(n_philo, table);
	table->start_time = get_time();
	table->running = 1;
	init_philo(n_philo, table);
	while (table->running && !all_satisfied(table))
		usleep(50);
	return (EXIT_SUCCESS);
}

static int	check_rules(t_table_rules rules, char **av, unsigned int n_philo)
{
	if (!ft_isnum(av[1]) || !ft_isnum(av[2]) || !ft_isnum(av[3])
		|| !ft_isnum(av[4]) || (rules.b_max_meal && !ft_isnum(av[5])))
		return (printf ("Error: invalid parameters are not digit.\n")
			* 0 + 1);
	if (rules.t_die > INT_MAX || rules.t_eat > INT_MAX
		|| rules.t_sleep > INT_MAX
		|| (rules.b_max_meal && rules.max_meal > INT_MAX))
		return (printf ("Error: invalid parameters are greater than MAX_INT.\n")
			* 0 + 1);
	if (rules.t_die < 1 || rules.t_eat < 1 || rules.t_sleep < 1
		|| (rules.b_max_meal && rules.max_meal < 1))
		return (printf ("Error: null or negatives values are not allowed.\n")
			* 0 + 1);
	if (n_philo <= 1)
		return (printf ("Error: at least 2 philosophers needed.\n")
			* 0 + 1);
	else if (n_philo > MAX_THREADS)
		return (printf("Error: too many philosophers.\n") * 0 + 1);
	return (0);
}

int	main(int ac, char **av)
{
	t_table_rules	rules;

	if (ac != 5 && ac != 6)
		return (printf ("Error: invalid parameters\n") * 0 + EXIT_FAILURE);
	rules.t_die = ft_atoi(av[2]);
	rules.t_eat = ft_atoi(av[3]);
	rules.t_sleep = ft_atoi(av[4]);
	rules.b_max_meal = 0;
	rules.max_meal = 0;
	if (ac == 6)
	{
		rules.b_max_meal = 1;
		rules.max_meal = ft_atoi(av[5]);
	}
	if (check_rules(rules, av, ft_atoi(av[1])))
	{
		return (EXIT_FAILURE);
	}
	return (init_table(rules, ft_atoi(av[1])));
}
