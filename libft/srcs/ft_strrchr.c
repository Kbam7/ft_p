/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 11:13:26 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/14 22:52:58 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				len;
	char			*ptr;

	ptr = (char *)s;
	len = ft_strlen(ptr);
	while (ptr[len] != c && len != -1)
		len--;
	if (len != -1 && ptr[len] == c)
		return (&ptr[len]);
	return (NULL);
}
