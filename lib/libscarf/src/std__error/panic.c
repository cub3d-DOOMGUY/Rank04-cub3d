#include "std__color.h"
#include "std__string.h"
#include "std__system.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void	std__panic_write_internal(t_const_string arr[])
{
	int	i;

	if (arr)
		std__writes(STDERR_FILENO, (t_const_string[]){
			BHRED "panic " BMAG "@", arr[0], HRED, NULL});
	else
	{
		std__write(STDERR_FILENO, BHRED "panic " BMAG
			"@std__panic_write_internal : arr is NULL\n" END);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (arr[++i])
		std__writes(STDERR_FILENO, (t_const_string[]){": ", arr[i], NULL});
	std__write(STDERR_FILENO, END "\n");
}

void	std__panic__syscall(t_const_string category)
{
	std__panic_write_internal(
		(t_const_string[]){category, strerror(errno), NULL});
	exit(EXIT_FAILURE);
}

// prints error and exits program
void	std__panic(t_const_string what)
{
	std__panic_write_internal((t_const_string[]){what, NULL});
	exit(EXIT_FAILURE);
}

void	std__panic__null(t_const_string where)
{
	std__panic_write_internal((t_const_string[]){where, " is NULL", NULL});
	exit(EXIT_FAILURE);
}
