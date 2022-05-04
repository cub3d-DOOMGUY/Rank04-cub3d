/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check__cubfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youkim    <youkim@student.42seoul.kr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 10:13:06 by youkim            #+#    #+#             */
/*   Updated: 2022/05/04 10:13:06 by youkim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std__string.h"

bool	cubfile__is_line_empty(t_string line)
{
	return (str__is_equal(line, "\n"));
}
