/*  Simple and fast CSV parser that is memory efficient and handles escaped characters and quotes correctly.
    Written by Shane Feek 04/20/2016  Email: shane.feek@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVLib.h"

int main()
{
    char **parsed;
    int numberOfFields;

    /* Our test string to parse */
    char *csv = "This is a field,,\"This line has a carriage\n return in it!\",Test,\"One\\Two,Three\",10,\\\"The End\\\"\n";

    /* Show it on the screen */
    printf("%s\n\n", csv);

    /* Parse it! */
    parsed = CSVParse(csv,&numberOfFields);

    /* Make sure that the parse was successful */
    if (parsed == NULL)
    {
        printf("String parsing failed!\n");
        return 1;
    }

    /* Show the parsed array */
    for(int i=0;i<numberOfFields;i++)
    {
        printf("Field %d: %s\n",i,parsed[i]);
    }

    /* Clean up after ourselves */
    cleanupStrings(parsed,numberOfFields);

    /* And we are out of here! */
    return 0;
}

/*  Parse CSV string
    Input:  String of CSV values
            Number of Fields container
    Return: Array of strings, one for each field
            Number of Fields
*/
char **CSVParse(char *str, int *numberOfFields)
{
    char *newStr = NULL;
    char currentCharacter;
    char **strArray = NULL;

    int quote = 0;
    int csvLength = strlen(str);
    int maxFieldCount = 2; /* Start with two fields as MAX */
    int *commaPositions = NULL;
    int currentField = 0;
    int cleanStringPosition = 0;
    int i;
    int startPosition;
    int fieldLength;

    /* Allocate memory for the comma position array */
    commaPositions = (int *) malloc(sizeof(int) * maxFieldCount);

    /* Allocate memory for "cleaned up" string the same size as the original string to guarantee that it is big enough */
    newStr = (char *) malloc(sizeof(char) * csvLength);

    /* Make sure that the memory allocation succeeded */
    if (commaPositions == NULL)
    {
        printf("Error Allocating Memory!\n");
        return NULL;
    }

    if (newStr == NULL)
    {
        printf("Error Allocating Memory!\n");
        return NULL;
    }

    /* First pass through to record the correct comma positions */
    for(i = 0; i < csvLength; i++)
    {
        /* Get a single character */
        currentCharacter = str[i];

        /* Skip any control or garbage characters */
        if (currentCharacter < 32) continue;

        /* Handle escape character not inside of quotes */
        if (currentCharacter == 92 && quote == 0)
        {
            /* Move ahead one character */
            i++;

            /* Keep the next good character */
            newStr[cleanStringPosition] = str[i];

            /* Move to the next good character position */
            cleanStringPosition++;

            /* Move on to the next character */
            continue;
        }

        /* Check for quote and keep track of pairs */
        if (currentCharacter == 34)
        {
            /* Toggle the pair indicator */
            quote = 1 - quote;
            /* Skip the quote */
            continue;
        }

        /* Check for comma that is NOT inside quotes */
        if (currentCharacter == 44 && quote == 0)
        {
            /* Check to see if we need to grow our comma position array */
            if (currentField >= maxFieldCount)
            {
                /* Double in size each time */
                maxFieldCount *= 2;

                /* Allocate more memory for the array*/
                commaPositions = (int *) realloc(commaPositions,sizeof(int) * maxFieldCount);

                /* Make sure that the memory allocation succeeded */
                if (commaPositions == NULL)
                {
                    printf("Error Expanding Array!\n");
                    return NULL;
                }
            }

            /* Keep track of the comma positions */
            commaPositions[currentField] = cleanStringPosition;

            /* Move to the next field */
            currentField++;
        }

        /* Keep the good characters */
        newStr[cleanStringPosition] = currentCharacter;

        /* Move to the next good character position */
        cleanStringPosition++;
    }

    /* Make sure that clean string gets NULL terminator */
    newStr[cleanStringPosition] = 0;

    /* Make sure to mark the end of the string as a "comma" position so that the last field gets included in the array */
    commaPositions[currentField]=cleanStringPosition;

    /* Include the last field in our count */
    currentField++;

    /* Record the Total number of fields to return to the calling function */
    *numberOfFields = currentField;

    /* Allocate an array of pointers to chars, not actually allocating any strings themselves */
    strArray = (char **) malloc(sizeof(char *) * currentField );

    /* Start at the beginning of the clean string */
    startPosition = 0;

    /* Copy the strings to the new string array */
    for (i = 0;i < currentField;i++)
    {
        /* Calculate length of the current field */
        fieldLength = commaPositions[i] - startPosition;

        /* Replace the comma with a Null terminator */
        newStr[commaPositions[i]] = 0;

        /* Allocate memory for the current field plus the Null Terminator */
        strArray[i] = (char *) malloc(sizeof(char) * (fieldLength + 1));

        /* Copy the string into the new array and include the Null Terminator */
        memcpy(strArray[i],newStr + startPosition,fieldLength + 1);

        /* Move our start position to the next field */
        startPosition = commaPositions[i] + 1;
    }

    /* Clean up the dynamic arrays */
    free(commaPositions);
    free(newStr);

    /* Return the new array back to the calling function */
    return strArray;
}

/*  Clean up an array of strings
    Input:  Array of strings
            Number of strings
    Return: none
*/
void cleanupStrings(char **strArray, int numberOfStrings)
{
    int i;

    /* Free the individual strings */
    for (i = 0; i < numberOfStrings; i++)
    {
        free (strArray[i]);
    }

    /* Once the strings themselves are freed, free the actual array itself */
    free (strArray);
}


