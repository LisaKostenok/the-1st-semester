#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

//---------------------------------------------------------------------------------------------------------------------
//!  \brief The function for opening file, reading it and creating array of pointers to strings from file
//!
//! @param[in] unsigned int* nlines - number of strings in file
//! @param[in] unsigned int* nsymb - number of symbols in file
//!
//! @return address of the array of pointers to strings
//!
//---------------------------------------------------------------------------------------------------------------------

char** readtext(unsigned int* nlines, unsigned int* nsymb);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for copying array of pointers to strings
//!
//! @param[in] const char** arrpointers - address of source array
//! @param[in] unsigned int nlines - number of elements of array
//!
//! @return address of array which is the copy of source array
//!
//---------------------------------------------------------------------------------------------------------------------

char** copytext(const char** arrpointers, unsigned int nlines);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for adding sorted or copied strings in new file
//!
//! @param[in] const char** arrpointers - address of array of pointers to strings
//! @param[in] unsigned int nlines - number of elements of array
//!
//! @return updated file
//!
//---------------------------------------------------------------------------------------------------------------------

int writetext(const char** arrpointers, unsigned int nlines);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for finding size of source text
//!
//! @param[in] FILE* text - address of source text
//!
//! @return size of the text in bytes
//!
//! @Note size is counting from the beginning to the end of file
//!
//---------------------------------------------------------------------------------------------------------------------

unsigned int findsize(FILE* text);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for creating buffer of all the symbols in the file
//!
//! @param[in] unsigned int nsymb - number of symbols in the file
//!
//! @return address of created buffer
//!
//---------------------------------------------------------------------------------------------------------------------

char* readfromfile(unsigned int* nsymb);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for counting strings from the text
//!
//! @param[in] const char** buffer - address of array of all the symbols in text
//!
//! @return number of strings
//!
//---------------------------------------------------------------------------------------------------------------------

unsigned int countlines(const char* buffer);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for creating array of pointers to strings
//!
//! @param[in] const char** buffer - address of array of all the symbols
//! @param[in] const char** arrpointers - address of free array
//! @param[in] unsigned int nsymb - number of strings
//!
//! @Note the result is arrpointers - array filled by pointers to strings
//!
//---------------------------------------------------------------------------------------------------------------------

int gopointers(char* buffer, char** arrpointers, unsigned int nsymb);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for comparing two strings in alphabetical order
//!
//! @param[in] const void* line1 - address of the first string
//! @param[in] const void* line2 - address of the second string
//!
//! @return 0 if lines are equal
//! @return plus number if line1 > line2
//! @return minus number if line1 < line2
//!
//! @Note the comparison goes from beginning of strings
//!
//---------------------------------------------------------------------------------------------------------------------

int compare(const void* line1, const void* line2);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for going from the beginning of the string to its end
//!
//! @param[in] char* line - address of the string
//!
//! @return number of symbols in the string
//!
//! @Note all symbols which are not letters are excluded
//!
//---------------------------------------------------------------------------------------------------------------------

unsigned int find_end(char** line);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for comparing two strings in alphabetical order
//!
//! @param[in] const void* line1 - address of the first string
//! @param[in] const void* line2 - address of the second string
//!
//! @return 0 if lines are equal
//! @return plus number if line1 > line2
//! @return minus number if line1 < line2
//!
//! @Note the comparison goes from ends of strings
//!
//---------------------------------------------------------------------------------------------------------------------

int compare_end(const void* line1, const void* line2);

//----------------------------------------------------------------------------------------------------------------------
//! \brief The function for freeing up memory
//!
//! @param[in] char** copy - address of the copy of source text
//! @param[in] char** arrpointers- address of array of pointers to strings
//!
//---------------------------------------------------------------------------------------------------------------------

void cleaner(char** copy, char** arrpointers);


int main()
{
    unsigned int nlines = 0, nsymb = 0;

    char** arrpointers =  readtext(&nlines, &nsymb);
    assert(arrpointers);

    char** copy = copytext(arrpointers, nlines);
    assert(copy);

    qsort(arrpointers, nlines, sizeof(char*), compare);
    writetext(arrpointers, nlines);

    qsort(arrpointers, nlines, sizeof(char*), compare_end);
    writetext(arrpointers, nlines);

    writetext(copy, nlines);

    cleaner(copy, arrpointers);
}


int compare(const void* line1, const void* line2)
{
    assert (line1);
    assert (line2);
    assert (line1 != line2);

    return strcmp(*(char**) line1, *(char**) line2);
}

unsigned int findsize(FILE* text)
{
    assert(text);

    fseek(text, 0, SEEK_END);
    unsigned int textsize = ftell(text);
    fseek(text, 0, SEEK_SET);
    return textsize;
}

char* readfromfile(unsigned int* nsymb)
{
    FILE* text = fopen("ONEGIN.txt", "r");

    assert(text);

    unsigned int textsize = findsize(text);
    char* buffer = (char*)calloc(textsize + 1, sizeof(char));

    assert(buffer);

    *nsymb = fread(buffer, sizeof(char), textsize + 1, text);
    fclose(text);
    return buffer;
}

unsigned int countlines(const char* buffer)
{
    assert (buffer);

    int num = 1;
    for (int i = 0; buffer[i] != '\0'; i++)
        if (buffer[i] == '\n')
            num++;
    return num;
}

int gopointers(char* buffer, char** arrpointers, unsigned int nsymb)
{
    assert (buffer);
    assert (arrpointers);

    unsigned int numpoint = 0;
    arrpointers[0] = &buffer[0];
    for (int i = 1; i < nsymb; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            if (i < nsymb - 1)
                arrpointers[++numpoint] = &buffer[i + 1];
        }
    return 0;
}

char** makelines(char* buffer, unsigned int* nlines, unsigned int nsymb)
{
    assert (buffer);

    *nlines = countlines(buffer);
    char** arrpointers = (char**) calloc(nlines, sizeof(*arrpointers));

    assert (arrpointers);

    gopointers(buffer, arrpointers, nsymb);

    return arrpointers;
}

char** readtext(unsigned int* nlines, unsigned int* nsymb)
{
    char* buffer = readfromfile(nsymb);
    return makelines(buffer, nlines, *nsymb);
}

int writetext(const char** arrpointers, unsigned int nlines)
{
    assert(arrpointers);

    FILE* newtext = fopen("NEWONEGIN.txt", "a");

    assert(newtext);

    for (int i = 0; i < nlines; i++)
        fprintf(newtext, "%s\n", arrpointers[i]);

    fprintf(newtext, "\n\n");
    fclose(newtext);
    return 0;
}

char** copytext(const char** arrpointers, unsigned int nlines)
{
    char** copy = (char **) calloc(nlines, sizeof(*copy));
    copy = memmove(copy, arrpointers, sizeof(char*)*nlines);
    return copy;
}

unsigned int find_end(char** line)
{
    assert (line);

    int length = 0;

    for (; **line != '\0'; length++)
        (*line)++;

    for (; isalpha(**line) == 0; length--)
        (*line)--;
    return length;
}

int compare_end(const void* str1, const void* str2)
{
    assert (str1);
    assert (str2);
    assert (str1 != str2);

    char* line1 = *(char**) str1;
    char* line2 = *(char**) str2;

    int len1 = find_end(&line1);
    int len2 = find_end(&line2);
    for (int i = 0; (i <= len1) && (i <= len2); i++)
    {
        if (*line1 != *line2)
            return *line1 - *line2;
        line1--;
        line2--;
    }
    return len1 - len2;
}

int addtext(const char** arrpointers, unsigned int nlines)
{
    assert(arrpointers);

    FILE* newtext = fopen("NEWONEGIN.txt", "a");

    assert(newtext);

    for (int i = 0; i < nlines; i++)
        fprintf(newtext, "%s\n", arrpointers[i]);
    fclose(newtext);
    return 0;
}

void cleaner(char** copy, char** arrpointers)
{
    free(copy);
    free(arrpointers);
}

