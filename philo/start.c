/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:55:32 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 16:10:23 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	eat_and_sleep(t_pdata *pdata, long long ms, long long *deathcounter)
{
	int	chunks_of_sleep;
	int	chunks_of_eat;

	chunks_of_sleep = pdata->timetosleep;
	chunks_of_eat = pdata->timetoeat;
	if (!waiting_to_eat(pdata, deathcounter))
		return (false);
	pthread_mutex_lock(&pdata->left_fork);
	pthread_mutex_lock(pdata->right_fork);
	if (!pr_eat_action(pdata, ms))
		return (unlock_mutex(pdata));
	ms = get_elapsed_time_ms(pdata->tv);
	gettimeofday(&pdata->checkifdead, NULL);
	if (!eat(pdata, deathcounter, chunks_of_eat))
		return (false);
	leave_forks(pdata);
	if (pdata->eating_counter == 0 || *pdata->dead)
		return (unlock_all_mutex(pdata));
	ms = get_elapsed_time_ms(pdata->tv);
	printf("%lld ", ms);
	printf("%d is sleeping\n", pdata->ph_n);
	unlock_all_mutex(pdata);
	if (!sleep_ac(pdata, deathcounter, chunks_of_sleep))
		return (false);
	return (true);
}

void	*start_thread(void *arg)
{
	t_pdata		*pdata;
	long long	ms;
	long long	deathcounter;
	bool		flag;

	pdata = (t_pdata *)arg;
	ms = 0;
	flag = true;
	deathcounter = pdata->timetodie;
	gettimeofday(&pdata->tv, NULL);
	gettimeofday(&pdata->checkifdead, NULL);
	while (true)
	{
		if (!thinking(pdata, ms))
			return (NULL);
		if (pdata->ph_n % 2 == 1)
		{
			if (!eat_and_sleep(pdata, ms, &deathcounter))
				return (NULL);
		}
		else
		{
			if (flag)
				usleep(8000);
			flag = false;
			if (!eat_and_sleep(pdata, ms, &deathcounter))
				return (NULL);
		}
	}
	return (NULL);
}

void	create_thread(pthread_t *philo, t_pdata *pdata, bool *death)
{
	int	i;

	i = 0;
	while (i < pdata->pcount)
	{
		pdata[i].dead = death;
		if (pthread_create(&philo[i], NULL, start_thread, pdata + i) != 0)
		{
			free(pdata);
			free(philo);
			return ;
		}
		i++;
	}
	i = 0;
	while (i < pdata->pcount)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	destroy_and_free(pdata, philo);
}

void	init_philo(t_pdata *pdata, pthread_mutex_t *pr, char **argv, int argc)
{
	int		i;
	int		counter;

	i = 0;
	counter = pdata->pcount;
	while (counter > 0)
	{
		if (argc == 6)
			pdata[i].eating_counter = ft_atoi(argv[5]);
		else
			pdata[i].eating_counter = 100000;
		pthread_mutex_init(&pdata[i].left_fork, NULL);
		pdata[i].right_fork = &pdata[i + 1].left_fork;
		pdata[i].lfork = false;
		pdata[i].rfork = &pdata[i + 1].lfork;
		pdata[i].pr = pr;
		pdata[i].ph_n = i + 1;
		pdata[i].timetosleep = ft_atoi(argv[4]);
		pdata[i].timetoeat = ft_atoi(argv[3]);
		pdata[i].timetodie = ft_atoi(argv[2]);
		i++;
		counter--;
	}
	pdata[i - 1].rfork = &pdata[0].lfork;
	pdata[i - 1].right_fork = &pdata[0].left_fork;
}
