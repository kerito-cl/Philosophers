/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2024/12/30 16:08:00 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_philo(t_pdata *pdata, int pcount)
{
    int i;

    i = 0;
    while (i < pcount)
    {
        i++;
        pdata->ph_n = i;
        printf("%d\n", pdata->ph_n);
    }
}

void* incrementCounter(void* arg) 
{
    t_pdata* pdata = (t_pdata*)arg;
    int i;

    i = 0;
    while (i < )
    {
        pthread_mutex_lock(&pdata->lfork);
        pdata->forks -= 2;
        pthread_mutex_unlock(&pdata->lfork);
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    pthread_t *philo;
    t_pdata *pdata;
    struct timeval tv;
    int pcount;
    int i;

    if (argc > 6 || argc < 5)
        return (0);
    parse(argv);
    pthread_mutex_init(&pdata->mutex, NULL);
    pcount = ft_atoi(argv[1]);
    pdata = malloc(sizeof(t_pdata) * pcount);
    pdata->pcount = pcount;
    init_philo(pdata, pcount);
    philo = malloc(pcount * sizeof(pthread_t));
    i = 0; 
    while (i < pcount)
    {
        pthread_create(&philo[i], NULL, incrementCounter, pdata);
        i++;
        printf("%d has taken a fork\n", i);
    }
    i = 0;
    while (i < pcount)
    {
        pthread_join(philo[i], NULL);
        i++;
    }
    /*if (gettimeofday(&tv, NULL) == 0) {
        printf("Seconds since Epoch: %ld\n", tv.tv_sec);
        printf("Microseconds: %ld\n", tv.tv_usec);
    } else {
        perror("gettimeofday failed");
    }*/
    pthread_mutex_destroy(&pdata->mutex);
    return 0;
}
