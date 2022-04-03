#include <unistd.h>

#include "h_file/minimake.h"

void print_help()
{
    printf("-f file\n-p\n-h");
    exit(0); // Sucess
}

void pretty_print(Variable *var, Rule *rule)
{
    Variable_print(var);
    Rule_print(rule);
    exit(0);
}

char *get_filename(int argc, char *argv[]) // return the filename
{ // exit if it isn't exit
    char *filename = NULL;

    for (int i = 1; i < argc; i++)
    {
        char *opt = argv[i];
        if (opt[0] == '-')
        {
            for (opt++; *opt; opt++)
            {
                switch (*opt)
                {
                case 'f':
                    if (i + 1 < argc)
                        filename = argv[i + 1];
                    i++;
                    break;
                }
            }
        }
    }

    if (filename != NULL)
    {
        if (access(filename, F_OK) != 0) // does not exits
            exit(2);
    }
    else
    {
        if (access("makefile", F_OK) == 0)
            filename = "makefile";
        else if (access("Makefile", F_OK) == 0)
            filename = "Makefile";
        else
            exit(2);
    }

    return filename;
}

int main(int argc, char *argv[])
{
    char *filename = get_filename(argc, argv);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        exit(1);

    Variable *var;
    Rule *rule;
    Parse(file, &var, &rule);

    Replace_Var(var);

    // pretty_print(var, rule);

    for (int i = 1; i < argc; i++)
    {
        char *opt = argv[i];
        if (opt[0] == '-')
        {
            for (opt++; *opt; opt++)
            {
                switch (*opt)
                {
                case 'f': // do nothing already done
                    i++;
                    break;
                case 'h':
                    print_help();
                    break;
                case 'p':
                    pretty_print(var, rule);
                    break;
                default:
                    exit(1); // The option is invalid
                    break;
                }
            }
        }
        else // it is a target
        {
            Execute_target(opt, rule);
        }
    }

    // TODO
    // execute first target if none have been executed

    // Variable_free(var);
    // Rule_free(rule);

    return 0;
}
