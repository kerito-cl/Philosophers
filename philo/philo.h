/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:19:39 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 16:03:46 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pdata
{
	pthread_mutex_t	*flag_mutex;
	pthread_mutex_t	forks;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*pr;
	struct timeval	tv;
	struct timeval	checkifdead;
	bool			*dead;
	bool			*rfork;
	bool			lfork;
	int				ph_n;
	int				pcount;
	int				eating_counter;
	int				timetosleep;
	int				timetoeat;
	int				timetodie;
}					t_pdata;

int					ft_atoi(const char *str);
void				parse(char **args);
void				destroy_and_free(t_pdata *pdata, pthread_t *philo);
bool				thinking(t_pdata *pdata, long long ms);
bool				waiting_to_eat(t_pdata *pdata, long long *deathcounter);
bool				pr_eat_action(t_pdata *pdata, long long ms);
bool				eat(t_pdata *pdata, long long *deathcounter,
						int chunks_of_eat);
bool				sleep_ac(t_pdata *pdata, long long *deathcounter,
						int chunks_of_sleep);
bool				death_checker(t_pdata *pdata, long long *deathcounter);
long long			get_elapsed_time_ms(struct timeval start_time);
bool				unlock_mutex(t_pdata *pdata);
bool				unlock_all_mutex(t_pdata *pdata);
void				leave_forks(t_pdata *pdata);
bool				eat_and_sleep(t_pdata *pdata, long long ms,
						long long *deathcounter);
void				*start_thread(void *arg);
void				create_thread(pthread_t *philo, t_pdata *pdata,
						bool *death);
void				init_philo(t_pdata *pdata, pthread_mutex_t *pr, char **argv,
						int argc);

#endif
