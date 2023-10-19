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
