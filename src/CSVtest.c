/*  Simple and fast CSV parser that is memory efficient and handles escaped characters and quotes correctly.
    Written by Shane Feek 04/20/2016  Email: shane.feek@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CSVLib.h>

int main()
{
	char **parsed;
	int numberOfFields;
	int i;
	/* Our test string to parse */
	char *csv = "This is a field,,\"This line has a carriage\n return in it!\",Test,\"One\\Two,Three\",10,\\\"The End\\\"\n";
	/* Show it on the screen */
	printf ("%s\n\n", csv);

	/* Parse it! */
	if (! (parsed = CSVParse (csv, &numberOfFields)))
	{
		printf ("String parsing failed!\n");
		return 1;
	}

	/* Show the parsed array */
	for (i = 0; i < numberOfFields; i++)
	{
		printf ("Field %d: %s\n", i, parsed[i]);
	}

	/* Clean up after ourselves */
	cleanupStrings (parsed, numberOfFields);
	/* And we are out of here! */
	return 0;
}
