#include "bsh.h"


void		realloc_buffer(t_input *input)
{
	char	*tmp;

	tmp = input->buffer;
	if (!(input->buffer = (char*)malloc((input->buffer_size * 2 + 1)
			* sizeof(char))))
		exit(EXIT_FAILURE);
	input->buffer_size *= 2;
	ft_bzero(input->buffer, input->buffer_size + 1);
	ft_strcpy(input->buffer, tmp);
	free(tmp);
}
