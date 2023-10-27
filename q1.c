#include<unistd.h>
#include<ctype.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LEN (8 * 1024)
#define MAX_LINES (8 * 1024)

char **sorted_file;
int cflag, uflag, iflag;

FILE * file(char *name, char *mode) {
	FILE *fp;
	if ((fp = fopen(name, mode)) == NULL)
		fprintf(stderr, "%s: %s", name, strerror(errno));
	return (fp);
}

int cmpfunc (const void * a, const void * b) {
   return strcmp(*(const char**)a,*(const char**)b);
}

void my_print(FILE *ofp, char *output_line, int repeats) {
    if (uflag) {
        if (repeats == 1) {
            if (cflag) {
                fprintf(ofp, "%4d %s", repeats, output_line);
            } else {
                fprintf(ofp, "%s", output_line);
            }
        }
    } else if (cflag) {
        fprintf(ofp, "%4d %s", repeats, output_line);
    } else if (repeats > 0) {
        fprintf(ofp, "%s", output_line);
    }
}

int main(int argc, char **argv) {
    // get command argument
    int cmd_opt = 0;
    int optcnt = 0;
    while ((cmd_opt = getopt(argc, argv, "-ciu")) != EOF) {
        switch (cmd_opt) {
            case 'c':
                //fprintf(stderr, "have c\n");
                cflag = 1;
                break;
            case 'i':
                //fprintf(stderr, "have i\n");
                iflag = 1;
                break;
            case 'u':
                //fprintf(stderr, "have u\n");
                uflag = 1;
                break;
            default:
                fprintf(stderr, "WARNNING: Not supported option\n");
                break;
        }
    }

    FILE *ifp, *ofp;

    ifp = file(argv[argc-1], "r");
    ofp = stdout;

    sorted_file = (char**)malloc(MAX_LINES);    
    char *tmpline = malloc(MAX_LINE_LEN);
    int idx = 0;
    while (fgets(tmpline, MAX_LINE_LEN, ifp)) {
        sorted_file[idx++] = tmpline;
        tmpline = malloc(MAX_LINE_LEN);
    }

    // sort
    qsort(sorted_file, idx, sizeof(char*), cmpfunc);

    //for (int i = 0; i < idx; i++) {
        //printf("%s", sorted_file[i]);
    //}

    // uniq
    int repeats = 0;
    register char *t1, *t2;
    char *prevline = malloc(MAX_LINE_LEN);
    char *thisline = malloc(MAX_LINE_LEN);

    prevline = sorted_file[0];

    for (int i = 1; i < idx; i++) {
        thisline = sorted_file[i];
        t1 = thisline;
        t2 = prevline;

        if (iflag) {
            t1 = malloc(strlen(thisline));
            t2 = malloc(strlen(prevline));
            memcpy(t1, thisline, strlen(thisline));
            memcpy(t2, prevline, strlen(prevline));
            for (int i = 0; t1[i] != '\0'; i++)
                t1[i] = tolower(t1[i]);
            for (int i = 0; t2[i] != '\0'; i++)
                t2[i] = tolower(t2[i]);
        }

        if (strcmp(t1, t2)) {
            my_print(ofp, prevline, repeats + 1);
            t1 = prevline;
			prevline = thisline;
			thisline = t1;
            repeats = 0;
        } else {
            ++repeats;
        }
    }
    my_print(ofp, prevline, repeats + 1);
}
