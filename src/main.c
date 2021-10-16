#include "philo.h"

t_table *g_table;

int	init_mutex(uint64_t n_forks)
{
	uint64_t	i;

	g_table->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * n_forks);
	if (!g_table->forks)
		return (EXIT_FAILURE);
	i = 0;
	while (i < n_forks)
		if (pthread_mutex_init(g_table->forks + i++, NULL) != 0)
			return (EXIT_FAILURE);
	if (pthread_mutex_init(&g_table->speak_lock, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void init_philo(uint64_t n_philo)
{
	uint64_t	i;

	i = 0;
	while (i < n_philo)
	{
		g_table->philos[i].forks[0] = g_table->forks + i;
		if (i < n_philo - 1)
			g_table->philos[i].forks[1] = g_table->forks + i + 1;
		else
			g_table->philos[i].forks[1] = g_table->forks;
		create_philo(g_table->philos + i);
        i++;
		usleep(50);
	}
}

static int	init_table(t_table_rules rules, uint64_t n_philo)
{
    g_table = malloc(sizeof(t_table));
	if (!g_table || n_philo <= 1)
		return (EXIT_FAILURE);
	g_table->rules = rules;
    g_table->philos = malloc(sizeof(t_philo) * n_philo);
	if (!g_table->philos)
		return (EXIT_FAILURE);
	g_table->n_philo = n_philo;
	init_mutex(n_philo);
    g_table->start_time = get_time();
	g_table->running = 1;
	init_philo(n_philo);
	while (g_table->running && (!rules.b_max_meal || !all_satisfied()))
		usleep(50);
	return (EXIT_SUCCESS);
}

static int	check_rules(t_table_rules rules, char **av, unsigned int n_philo)
{
	if (!ft_isnum(av[1]) || !ft_isnum(av[2]) || !ft_isnum(av[3])
		|| !ft_isnum(av[4]) || (rules.b_max_meal && !ft_isnum(av[5])))
		return (printf ("Error: invalid parameters are non numeric.\n")
			* 0 + 1);
	if (rules.t_die > INT_MAX || rules.t_eat > INT_MAX
		|| rules.t_sleep > INT_MAX
		|| (rules.b_max_meal && rules.max_meal > INT_MAX))
		return (printf ("Error: invalid parameters goes above MAX_INT.\n")
			* 0 + 1);
	if (rules.t_die < 1 || rules.t_eat < 1 || rules.t_sleep < 1
		|| (rules.b_max_meal && rules.max_meal < 1))
		return (printf ("Error: null or negatives values are not allowed.\n")
			* 0 + 1);
	if (n_philo <= 1)
		return (printf ("Error: at least 2 philosophers are required.\n")
			* 0 + 1);
	else if (n_philo > MAX_THREADS)
		return (printf("Error: too many philosophers.\n") * 0 + 1);
	return (0);
}

int main(int ac, char **av)
{
    t_table_rules rules;

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