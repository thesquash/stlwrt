#include <stdio.h>

#define		is_space_character(i)		\
		(i >=  0x00 && i <= 0x20)

#define		is_punctuation_character(i)	\
		((i >= 0x21 && i <= 0x2f) ||	\
		 (i >= 0x3a && i <= 0x40) ||	\
		 (i >= 0x5b && i <= 0x60) ||	\
		 (i >= 0x7b && i <= 0x7e)

#define		is_effective_separator(i)	\
		(is_space_character(i) ||	\
		 is_punctuation_character(i))


void		process_line(const char *line_of_code);


int		main(int argc, char **argv)
{
	FILE	*input = NULL;
	char	line_of_code[160];
	
	
	if (argc < 2 || strcmp(argv[1], "gtk") || strcmp(argv[1], "gdk"))
	{
		fprintf(stderr,
"Specify either \"gtk\" or \"gdk\" as a parameter.\n");
		return 1;
	}
	
		
	
	while (fgets(line_of_code, 160, stdin) != NULL)
		process_line(line_of_code);
	
	return;
}


void		process_line(const char *line)
{
	unsigned int	line_index = 0;
	
	static enum
	{
		RETURN_TYPE,
		FUNCTION_NAME,
		PARAMETERS,
		ATTRIBUTES
	}		function_processing_state;
	
	int		non_space_character_encountered = 0;
	int		token_begun = 0;
	
	
	
	static char	return_type[128];
	
	static char	function_name[128];
	
	static struct
	{
		char	parameter_type[128];
		char	parameter_name[128];
	}		parameters[8];
	
	static char	attributes[128];
	
	
	for (line_index = 0; line[line_index] != '\0'; line_index++)
	{
		if (is_space_character(line[line_index]))
		{
			/* [...] */
			continue;
		}

		/* If we get here, this is not a space character; this is a
		   token (or partial token) of some kind. */
		
		if (!non_space_character_encountered && line[line_index] == '#')
		{
			
		}
	}
}
