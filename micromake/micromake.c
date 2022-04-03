#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int isname(int c) // 1 if c can be in a name
{
    return c != 0 && !isspace(c) && c != ':' && c != '=';
}

int isEOL(int c) // add # the to manage comment
{
    return c == '\n' || c == 0;
}

static void skip_class(int (*classifier)(int c), char **cursor)
{
    while (classifier(**cursor))
        (*cursor)++;
}

char *extract_word(char **ptr_str)
{
    // Return the start of a word and stock in ptr_str the end
    // As an example:" foo" -> *ptr_str+1 && *ptr_str = *ptr_str+3

    skip_class(isspace, ptr_str);
    char *word_start = *ptr_str; // the beginning of the word

    skip_class(isname, ptr_str);

    return word_start;
}

int check_validity(char *line) // 1 if it is not valide
{
    if (*line == '\t')
    {
        for (; *line == '\t'; line++)
            continue;
        if (isEOL(*line))
            return 1;
    }
    else
    {
        char *word_start = extract_word(&line);
        char *word_end = line;

        if (word_start == word_end) // line is empty
            return 0;

        size_t i = 0;
        for (; line[i] != '\r' && !isEOL(line[i]); i++)
            continue;

        if (line[i] == '\r' && !isEOL(line[i + 1]))
            return 1;
    }
    return 0;
}

int parse_rule(FILE *file) // return 1 if fail
{
    char *line = NULL;
    size_t line_len;
    int res = getline(&line, &line_len, file);
    char *save = line; // else bug zhen modify line

    if (res <= 0)
        return 1;

    if (check_validity(line))
    {
        free(save);
        err(1, NULL);
    }

    char *word_start = extract_word(&line);
    char *word_end = line;

    if (word_start != word_end) // only if the line is not empty
    {
        printf("[%.*s]:", (int)(word_end - word_start), word_start);

        word_start = extract_word(&line);
        line++; // both line together allow to skip ':' or '='
        if (*word_start != ':') // if there is no ':' or '=' exit
        {
            free(save);
            err(1, NULL);
        }

        while (*line)
        {
            word_start = extract_word(&line);
            word_end = line;

            if (word_start == word_end)
                break;

            printf(" (%.*s)", (int)(word_end - word_start), word_start);
        }
        printf("\n");
    }

    free(save);
    return res > 0 ? 0 : 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) // fail
        return 1;

    while (!parse_rule(file))
        ;

    fclose(file);

    return 0;
}
