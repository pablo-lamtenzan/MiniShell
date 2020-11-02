/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_lesser_redir_test.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 01:27:30 by pablo             #+#    #+#             */
/*   Updated: 2020/11/02 02:03:30 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
# define BUFFER_SIZE 1024

// i think is just this (if read waits)
// when this fct returns the previous cmd must be executed next
/* minish> $ ls << cat
> ca
> at
> cat
ls stuffffff
*/
void                handle_double_rev_redir(char* pattern, ssize_t pattern_size)
{
    char			buff[BUFFER_SIZE];
    ssize_t			bytes_read;

    while (bytes_read = read(STDOUT_FILENO, &buff, pattern_size))
    {
		ft_dprintf(STDOUT_FILENO, "\n%s", "> ");
		if (!ft_strncmp(buff, pattern, pattern_size))
			return ;
		ft_bzero(&buff, BUFFER_SIZE);
    }
}