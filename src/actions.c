#include "philo.h"

uint64_t	philo_talk(t_philo *philo, int action_id)
{
	uint64_t	time;
	static char	*action[5] = {
		"Has taken a fork",
		"Is eating",
		"Is sleeping",
		"Is thinking",
		"died"
	};

	if (pthread_mutex_lock(&g_table->speak_lock) != 0)
		return (EXIT_FAILURE);
	time = get_time();
	if (time < 0)
		time = 0;
	printf ("\033[1;34m%8lu \033[1;32m%2lu \033[1;37m-> \033[3;30m%s\033[0m\n",
		time - g_table->start_time, philo->id, action[action_id]);
	if (pthread_mutex_unlock(&g_table->speak_lock) != 0)
		return (EXIT_FAILURE);
	return (time);
}

unsigned char	philo_take_forks(t_philo *philo)
{
	if (pthread_mutex_lock(philo->forks[0]) != 0)
		return (EXIT_FAILURE);
    if (philo_talk(philo, A_FORK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(philo->forks[1]) != 0)
		return (EXIT_FAILURE);
    if (philo_talk(philo, A_FORK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

unsigned char	philo_eat(t_philo *philo)
{
	if (pthread_mutex_lock(philo->lock) != 0)
		return (EXIT_FAILURE);
	philo->t_last_eat = philo_talk(philo, A_EAT);
	if (g_table->rules.b_max_meal)
		philo->n_meal++;
	if (pthread_mutex_unlock(philo->lock) != 0)
		return (EXIT_FAILURE);
	sleep_until(philo->t_last_eat + g_table->rules.t_eat);
	if (pthread_mutex_unlock(philo->forks[0]) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_unlock(philo->forks[1]) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

unsigned char	philo_sleep(t_philo *philo)
{
	sleep_until(philo_talk(philo, A_SLEEP) + g_table->rules.t_sleep);
	philo_talk(philo, A_THINK);
	return (EXIT_SUCCESS);
}

int	all_satisfied(void)
{
	uint64_t		satisfied;
	unsigned char	i;

	i = 0;
	satisfied = 0;
	while (i < g_table->n_philo)
		satisfied += g_table->philos[i++].satisfied;
	if (satisfied == g_table->n_philo)
		return (1);
	return (0);
}