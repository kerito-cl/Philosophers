/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlock.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:51:11 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 16:10:47 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	unlock_mutex(t_pdata *pdata)
{
	pthread_mutex_unlock(&pdata->left_fork);
	pthread_mutex_unlock(pdata->right_fork);
	return (false);
}

bool	unlock_all_mutex(t_pdata *pdata)
{
	pthread_mutex_unlock(&pdata->left_fork);
	pthread_mutex_unlock(pdata->right_fork);
	pthread_mutex_unlock(pdata->pr);
	return (false);
}

void	leave_forks(t_pdata *pdata)
{
	pdata->eating_counter--;
	pthread_mutex_lock(pdata->pr);
	pdata->lfork = false;
	*pdata->rfork = false;
	pthread_mutex_unlock(pdata->pr);
	pthread_mutex_lock(pdata->pr);
}

void	destroy_and_free(t_pdata *pdata, pthread_t *philo)
{
	int		i;

	i = 0;
	while (i < pdata->pcount)
	{
		pthread_mutex_destroy(&pdata[i].left_fork);
		i++;
	}
	free(pdata);
	free(philo);
}
