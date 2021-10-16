#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>

# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include "limits.h"

# define    A_FORK  0
# define    A_EAT   1
# define    A_SLEEP 2
# define    A_THINK 3
# define    A_DIE   4

# define    MAX_THREADS 256

/*
**  Philo
*/

typedef struct s_philo
{
	uint64_t		id;
	uint64_t		t_birth;
	uint64_t		t_last_eat;
	uint64_t		n_meal;
	unsigned char	satisfied;
	pthread_t		thread;
	pthread_t		death_observer;
	pthread_mutex_t	*forks[2];
	pthread_mutex_t	*lock;
}				t_philo;

unsigned char		create_philo(t_philo *philo);
uint64_t			philo_talk(t_philo *const philo, int action_id);
unsigned char		philo_take_forks(t_philo *const philo);
unsigned char		philo_eat(t_philo *const philo);
unsigned char		philo_sleep(t_philo *const philo);
int					all_satisfied(void);

/*
**  Table and rules
*/

typedef struct s_table_rules
{
	unsigned int	t_die;
	unsigned int	t_eat;
	unsigned int	t_sleep;
	unsigned int	max_meal;
	unsigned char	b_max_meal;
}				t_table_rules;

typedef struct s_table
{
	unsigned char	running;
	uint64_t		start_time;
	uint64_t		n_philo;
	t_philo			*philos;
	pthread_mutex_t	speak_lock;
	pthread_mutex_t	*forks;
	t_table_rules	rules;
}				t_table;

extern t_table	*g_table;

/*
**  utils
*/

uint64_t		ft_atoi(char *s);
unsigned char	ft_isnum(char *s);
void			*nmalloc(void **ptr, size_t size, size_t block);
uint64_t		get_time(void);
uint64_t		get_time_since(const uint64_t time);
void			sleep_until(const uint64_t time);

#endif