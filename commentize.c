/*
 * This software is hereby released into the public domain.
 * You are free to use it in any way whatsoever.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum comment_style {
    STYLE_C,
    STYLE_C_ALT,
    STYLE_PYTHON
};

struct options {
    const char *inpath;
    const char *outpath;  /* stdout if NULL */
    int style;
    int linelen;
};

static struct options args = {
    NULL,
    NULL,
    STYLE_C,
    80
};

static const char* exename;

static void usage () {
    printf("Usage: %s <input file> [-s | --style] [c | c-alt | python]\n"
           "[-o | --output <output file>] [-n | --line-length <characters>]\n",
           exename);
    exit(-1);
}

static int parse_args (const char **remaining) {
    const char *first, *second;

    first = *remaining;
    if (first[0] == '-') {
        switch (first[1]) {
            case 's': goto STYLE;
            case 'o': goto OUTFILE;
            case 'n': goto LINELEN;
            case '-': {
                if (!strcmp(first, "--style"))
                    goto STYLE;
                else if (!strcmp(first, "--output"))
                    goto OUTFILE;
                else if (!strcmp(first, "--line-length"))
                    goto LINELEN;
                break;
            }
            default:
                usage();
                return 0;
        }
    } else {
        args.inpath = first;
        return 1;
    }

    STYLE:
    second = remaining[1];
    if (!strcmp(second, "c"))
        args.style = STYLE_C;
    else if (!strcmp(second, "c-alt"))
        args.style = STYLE_C_ALT;
    else if (!strcmp(second, "python"))
        args.style = STYLE_PYTHON;
    else
        usage();
    return 2;

    OUTFILE:
    args.outpath = remaining[1];
    return 2;

    LINELEN:
    args.linelen = atoi(remaining[1]);
    return 2;
}

void commentize (int argc, const char **argv) {
    if (!argc) usage();
    while (argc) {
        int args_read = parse_args(argv);
        argc -= args_read;
        argv += args_read;
    }
    const char *beginning, *ending, *prefix;
    switch (args.style) {
        case STYLE_C:
            beginning = "/*\n";
            ending = " */\n";
            prefix = " * %s\n";
            break;
        case STYLE_C_ALT:
            beginning = "";
            ending = "";
            prefix = "// %s\n";
            break;
        case STYLE_PYTHON:
            beginning = "###\n";
            ending = "###\n";
            prefix = "# %s\n";
            break;
        default:
            break;
    }

    FILE *infile = (args.inpath ? fopen(args.inpath, "r") : stdin);
    FILE *outfile = (args.outpath ? fopen(args.outpath, "w") : stdout);
    fputs(beginning, outfile);
    static char buffer[128];
    char *lastspace = NULL;
    int bufpos = 0;
    int linelen = args.linelen - strlen(prefix);
    int cons_newlines = 0;
    while (1) {
        int cc = fgetc(infile);
        if (cc == ' ' || cc == 0) {
            buffer[bufpos] = 0;
            if (lastspace) *lastspace = ' ';
            lastspace = buffer + bufpos;
            cons_newlines = 0;
            bufpos++;
        } else if (cc == EOF) {
            *lastspace = ' ';
            buffer[bufpos] = 0;
            fprintf(outfile, prefix, buffer);
            break;
        } else if (bufpos > linelen) {
            //buffer[bufpos] = 0;
            fprintf(outfile, prefix, buffer);
            int start = strlen(++lastspace);
            memmove(buffer, lastspace, start + 1);
            bufpos = start;
            if (cc != '\n' && cc != '\r') {
                buffer[bufpos++] = cc;
                cons_newlines = 0;
            } else cons_newlines++;
            lastspace = NULL;
        } else if (cc == '\n') {
            buffer[bufpos] = 0;
            if (++cons_newlines == 1) {
                if (lastspace) *lastspace = ' ';
                lastspace = buffer + bufpos;
                bufpos++;
            } else if (cons_newlines == 2) {
                fprintf(outfile, prefix, buffer);
                fprintf(outfile, prefix, "");
                bufpos = 0;
                lastspace = NULL;
            } else {
                fprintf(outfile, prefix, "");
                bufpos = 0;
                lastspace = NULL;
            }
        } else if (cc != '\r') {
            buffer[bufpos++] = cc;
            cons_newlines = 0;
        }
    }
    fputs(ending, outfile);
}

int main (int argc, const char **argv) {
    exename = argv[0];
    commentize(argc - 1, argv + 1);
    return 0;
}
