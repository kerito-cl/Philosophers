/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2025/01/03 16:51:13 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_elapsed_time_ms(struct timeval start_time) 
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    
    long long elapsed_seconds = current_time.tv_sec - start_time.tv_sec;
    long long elapsed_microseconds = current_time.tv_usec - start_time.tv_usec;

    return (elapsed_seconds * 1000) + (elapsed_microseconds / 1000);
}

void    pr_eat_action(t_pdata *pdata, long long ms)
{
        pthread_mutex_lock(pdata->pr);
        pdata->left_fork = true;
        pdata->right_fork = true;
        pdata[pdata->next].right_fork = true;
        printf("%lld ", ms);
        printf("%d has taken a fork\n", pdata->ph_n);
        printf("%lld ", ms);
        printf("%d has taken a fork\n", pdata->ph_n);
        pthread_mutex_unlock(pdata->pr);
}

void eat(t_pdata *pdata, long long ms)
{
    pthread_mutex_lock(&pdata->forks);
    pthread_mutex_lock(&pdata[pdata->next].forks);
    pr_eat_action(pdata, ms);
    printf("%lld ", ms);
    printf("%d is eating\n", pdata->ph_n);
    usleep(pdata->timetoeat);
    pdata->left_fork = false;
    pdata->right_fork = false;
    pdata[pdata->next].right_fork = false;
    pthread_mutex_unlock(&pdata[pdata->next].forks);
    pthread_mutex_unlock(&pdata->forks);
    pdata->sleeping = true;
    printf("%lld ", ms);
    printf("%d is sleeping\n", pdata->ph_n);
    usleep(pdata->timetosleep);
}
int len(t_pdata *pdata)
{
    int counter;

    counter = 0;
    while(pdata[counter].end != true)
        counter++;
    return (counter + 1);
}
void init_philo(t_pdata *pdata, int pcount, pthread_mutex_t *pr, char **argv)
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
        pdata[i].timetosleep = ft_atoi(argv[4]) * 1000;
        pdata[i].timetoeat = ft_atoi(argv[3]) * 1000;
        pdata[i].timetodie = ft_atoi(argv[2]) * 1000;
        pdata[i].checkifdead = ft_atoi(argv[2]) * 1000;

        i++;
        counter--;
    }
    pdata[i - 1].next = 1 - pcount;
    pdata[i - 1].end = true;
    pdata[i - 1].right_fork = false;
}

void* start_thread(void* arg) 
{
    t_pdata* pdata = (t_pdata*)arg;
    int start_time;
    int current_time;
    long long ms;

    gettimeofday(&pdata->tv, NULL);
    start_time = pdata->tv.tv_sec;
    //time = pdata->tv.tv_sec - time;

    while (true)
    {
        //gettimeofday(&pdata->tv, NULL);
        pthread_mutex_lock(pdata->pr);
        ms = get_elapsed_time_ms(pdata->tv);
        printf("%lld ", ms);
        printf("%d is thinking\n", pdata->ph_n);
        pthread_mutex_unlock(pdata->pr);
        eat(pdata, ms);
        /*if (gettimeofday(&pdata->tv, NULL) == 0) 
        {
            pdata->checkifdead = pdata->checkifdead - pdata->tv.tv_sec;
            printf("Microseconds: %u\n", );
        } else {
            perror("gettimeofday failed");
        }*/
        //usleep(200000);
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    pthread_t *philo;
    t_pdata *pdata;
    int pcount;
    pthread_mutex_t pr;
    int i;

    if (argc > 6 || argc < 5)
        return (0);
    parse(argv);
    pcount = ft_atoi(argv[1]);
    pdata = malloc(sizeof(t_pdata) * pcount + 2);
    pdata->pcount = pcount;
    init_philo(pdata, pcount, &pr,argv);
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
    //pthread_mutex_destroy(&pdata->lfork);
    return 0;
}
