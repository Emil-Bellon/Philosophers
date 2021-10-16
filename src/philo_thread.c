#include "philo.h"

static void	*death_observer_routine(void *const arg)
{
	t_philo	*self;

	self = (t_philo *)arg;
	while (!self->satisfied && g_table->running)
	{
		if (pthread_mutex_lock(self->lock) != 0)
			return (NULL);
		if (get_time_since(self->t_last_eat) >= g_table->rules.t_die)
		{
			philo_talk(self, A_DIE);
			pthread_mutex_lock(&g_table->speak_lock);
			g_table->running = 0;
			pthread_mutex_unlock(self->lock);
			return (NULL);
		}
		if (!self->satisfied && g_table->rules.b_max_meal
			&& self->n_meal == g_table->rules.max_meal)
			self->satisfied = 1;
		if (pthread_mutex_unlock(self->lock) != 0)
			return (NULL);
	}
	return (arg);
}

static void	*philo_routine(void *const arg)
{
	t_philo	*self;

	self = (t_philo *)arg;
	while (!self->satisfied && g_table->running)
	{
		if (philo_take_forks(self) == EXIT_FAILURE)
		{
			if (pthread_mutex_unlock(self->forks[0]) != 0
				|| pthread_mutex_unlock(self->forks[1]) != 0)
				return (NULL);
		}
		if (philo_eat(self) == EXIT_FAILURE)
			return (NULL);
		if (!self->satisfied && philo_sleep(self) == EXIT_FAILURE)
			return (NULL);
		if (g_table->rules.b_max_meal
			&& self->n_meal >= g_table->rules.max_meal)
			return (NULL);
	}
	return (arg);
}

unsigned char	create_philo(t_philo *philo)
{
	static uint64_t	philo_id = 0;

	philo_id++;
	philo->id = philo_id;
	philo->t_birth = get_time();
	philo->t_last_eat = philo->t_birth;
	philo->n_meal = 0;
	philo->satisfied = 0;
	philo->lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (philo->lock && pthread_mutex_init(philo->lock, NULL) != 0)
		return (EXIT_FAILURE);
	if ((pthread_create(&philo->thread, NULL, philo_routine, philo)) != 0)
		return (EXIT_FAILURE);
	if ((pthread_detach(philo->thread)) != 0)
		return (EXIT_FAILURE);
	if ((pthread_create(&philo->death_observer, NULL,
				death_observer_routine, philo)) != 0)
		return (EXIT_FAILURE);
	if ((pthread_detach(philo->death_observer)) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}