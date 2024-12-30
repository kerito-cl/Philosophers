/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:19:39 by mquero            #+#    #+#             */
/*   Updated: 2024/12/30 16:07:44 by mquero           ###   ########.fr       */
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
    pthread_mutex_t lfork;
    pthread_mutex_t rfork;
    int ph_n;
    int pcount;
    bool    left_fork;
    bool    right_fork;
}			t_pdata;

char		*ft_strjoin(char const *s1, char const *s2);
void		freesplit(char **strs);
char		**ft_split(char const *s, char c);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strdup(const char *s);
int	        ft_atoi(const char *str);
void	parse(char **args);

#endif