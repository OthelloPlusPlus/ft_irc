/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   verboseCheck.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/19 20:25:12 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/10/19 20:25:13 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERBOSECHECK_H
# define VERBOSECHECK_H

enum	e_verbose
{
	V_ERR = -1,
	V_SILENT = 0,
	V_ADMIN,
	V_DETAILS,
	V_CHANNEL,
	V_USER,
	V_MSG
};

int verboseCheck(void);

#endif
