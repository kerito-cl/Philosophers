/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2025/01/02 20:03:32 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    pr_eat_action(t_pdata *pdata)
{
        pthread_mutex_lock(pdata->pr);
        pdata->left_fork = true;
        pdata->right_fork = true;
        pdata[pdata->next].right_fork = true;
        printf("%d has taken a fork\n", pdata->ph_n);
        printf("%d has taken a fork\n", pdata->ph_n);
        pthread_mutex_unlock(pdata->pr);
}

void eat(t_pdata *pdata)
{
    pthread_mutex_lock(&pdata->forks);
    pthread_mutex_lock(&pdata[pdata->next].forks);
    pr_eat_action(pdata);
    printf("%d is eating\n", pdata->ph_n);
    usleep(200000);
    pdata->left_fork = false;
    pdata->right_fork = false;
    pdata[pdata->next].right_fork = false;
    pthread_mutex_unlock(&pdata[pdata->next].forks);
    pthread_mutex_unlock(&pdata->forks);
    pdata->sleeping = true;
    printf("%d is sleeping\n", pdata->ph_n);
    usleep(200000);
}
int len(t_pdata *pdata)
{
    int counter;

    counter = 0;
    while(pdata[counter].end != true)
        counter++;
    return (counter + 1);
}
void init_philo(t_pdata *pdata, int pcount, pthread_mutex_t *pr)
{
    int i;
    int counter;

    i = 0;
    counter = pcount;
    pthread_mutex_init(pr, NULL);
    while (counter > 0)
    {
        pdata[i].index = i;
        pdata[i].pr = pr;
        pthread_mutex_init(&pdata[i].forks, NULL);
        pdata[i].ph_n = i + 1;
        pdata[i].next = 1;
        pdata[i].left_fork = false;
        pdata[i].right_fork = false;
        pdata[i].sleeping = false;
        pdata[i].dead = false;
        pdata[i].eating = false;
        pdata[i].thinking = false;
        pdata[i].end = false;
        i++;
        counter--;
    }
    pdata[i - 1].next = 1 - pcount;
    //printf("%d", pdata[i - 1].next);
    pdata[i - 1].end = true;
    pdata[i - 1].right_fork = false;
}

void* start_thread(void* arg) 
{
    t_pdata* pdata = (t_pdata*)arg;
    int counter;

    while (true)
    {
            printf("%d is thinking\n", pdata->ph_n);
            eat(pdata);
        //usleep(200000);
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    pthread_t *philo;
    t_pdata *pdata;
    struct timeval tv;
    int pcount;
    pthread_mutex_t pr;
    int i;

    if (argc > 6 || argc < 5)
        return (0);
    parse(argv);
    pcount = ft_atoi(argv[1]);
    pdata = malloc(sizeof(t_pdata) * pcount + 2);
    pdata->pcount = pcount;
    init_philo(pdata, pcount, &pr);
    philo = malloc(pcount * sizeof(pthread_t));
    i = 0; 
    while (i < pcount)
    {
        pthread_create(&philo[i], NULL, start_thread, pdata + i);
        i++;
    }
    i = 0;
    while (i < pcount)
    {
        pthread_join(philo[i], NULL);
        i++;
    }
    i = 0;
    while (i < pcount)
    {
        pthread_mutex_destroy(&pdata[i].forks);
        i++;
    }
    pthread_mutex_destroy(&pr);
    /*if (gettimeofday(&tv, NULL) == 0) {
        printf("Seconds since Epoch: %ld\n", tv.tv_sec);
        printf("Microseconds: %ld\n", tv.tv_usec);
    } else {
        perror("gettimeofday failed");
    }*/
    //pthread_mutex_destroy(&pdata->lfork);
    return 0;
}
