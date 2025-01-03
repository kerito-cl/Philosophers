/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:19:39 by mquero            #+#    #+#             */
/*   Updated: 2025/01/03 13:58:57 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <pthread.h>
# include <unistd.h>
#include <sys/time.h>


typedef struct s_pdata
{
    pthread_mutex_t mutex;
    pthread_mutex_t forks;
    pthread_mutex_t *pr;
    struct timeval tv;
    int ph_n;
    int pcount;
    int index;
    int next;
    bool    left_fork;
    bool    right_fork;
    bool    sleeping;
    bool    thinking;
    bool    eating;
    bool    dead;
    bool    end;
    unsigned int    timetosleep;
    unsigned int    timetoeat;
    unsigned int    timetodie;
    unsigned int    checkifdead;
}			t_pdata;

char		*ft_strjoin(char const *s1, char const *s2);
void		freesplit(char **strs);
char		**ft_split(char const *s, char c);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strdup(const char *s);
int	        ft_atoi(const char *str);
void	parse(char **args);

#endif