#include "shell.h"

/**
 * parse_user_input - Buidls an array of strings as arguments
 * @str_input: Command input given by the user
 * @delimiter: String od chars indicating the delimiters
 *
 * Return: Array of strings
*/
char **parse_user_input(char *str_input, char *delimiter)
{
	int i, args_count = 0;
	char **args;
	char *token, *tkn_ptr;
	char *str_copy;

	if (str_input == NULL)
		dispatch_error("Error while parsing the command\n", 100);

	/* Count the number of arguments present in the input */
	args_count = count_args(str_input, delimiter);
	/* Allocate memory to hold eaach argument as a string */
	args = allocate_memory(sizeof(char *) * (args_count + 1));
	/* Store each argument as a string */
	str_copy = duplicate_string(str_input);
	tkn_ptr = str_copy;
	for (i = 0; i < args_count; i++)
	{
		token = _strtok(tkn_ptr, delimiter);
		if (token == NULL)
			break;
		tkn_ptr = NULL;
		/* store command as single string */
		args[i] = duplicate_string(token);
	}
	/* set the last element of array of arguments to NULL */
	args[i] = NULL;

	free(str_copy);

	return (args);
}

/**
 * count_args - Counts the number of arguments in a command string
 * @str_input: Command as a string
 * @delimiter: Delimiter by wich to separate the args
 *
 * Return: Number of arguments present in str_input
*/
int count_args(char *str_input, char *delimiter)
{
	char *tkn, *tkn_ptr;
	int count = 0;
	char *str_copy = duplicate_string(str_input);

	tkn_ptr = str_copy;
	while ((tkn = _strtok(tkn_ptr, delimiter)) != NULL)
	{
		count++;
		tkn_ptr = NULL;
	}

	free(str_copy);

	return (count);
}

/**
 * handle_PATH - Checks if the command to execute could be found in PATH's dirs
 * @commands: Array of strings containing the command and args
 * Return: flag 0 if is succes; -1 if is error
*/
int handle_PATH(char **commands)
{
	char *path_dirs, *path;
	char *tkn, *tkn_ptr;
	char *str_copy;
	int flag = -1;

	if (
		commands == NULL || commands[0] == NULL ||
		commands[0][0] == '\0' || commands[0][0] == '/'
	)
		return(-1);

	path_dirs = getenv("PATH");
	if (path_dirs == NULL)
	{
		dispatch_error("Error", 100);
		return (-1);
	}

	str_copy = duplicate_string(path_dirs);
	tkn_ptr = str_copy;
	while (1)
	{
		tkn = _strtok(tkn_ptr, ":");
		if (tkn == NULL)
			break;
		tkn_ptr = NULL;

		path = getpath(tkn, commands[0]);
		if (_strcmp(path, "help") == 0)
		{
			free(commands[0]);
			commands[0] = path;
			flag = 1;
			puts("I am here HELPPPPP\n");
			break;
		}
		else if (access(path, F_OK) != -1)
		{
			free(commands[0]);
			commands[0] = path;
			flag = 1;
			break;
		}
		free(path);
	}
	free(str_copy);
	return (flag);
}

/**
 * getpath - Creates a string representing a full path to file
 * @dir: String representing a directory path
 * @filename: Name of the file we are looking for
 *
 * Return: String as a full path to "filename"
*/
char *getpath(char *dir, char *filename)
{
	int dir_len = strlen(dir);
	int filename_len = strlen(filename);
	char *path;

	path = allocate_memory(sizeof(char *) * (dir_len + filename_len + 2));

	strcpy(path, dir);
	strcat(path, "/");
	strncat(path, filename, filename_len + 1);

	return (path);
}
