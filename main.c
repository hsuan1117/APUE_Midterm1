
#include <sys/times.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BSZ 1024

struct memstream
{
    size_t pos;
    size_t size;
    char *buf;
};
typedef struct memstream mst_t;

static int readfn(void *mst, char *buf, int size){
    mst_t* mem = mst;
    size_t available = mem->size - mem->pos;
    if (size > available)
    {
        size = available;
    }
    memcpy(buf, mem->buf, size*sizeof(char));
    mem->pos = mem->pos + size;
    return size;
}

static int writefn(void *mst, const char *buf, int size){
    mst_t *mem = mst;
    size_t available = mem->size - mem->pos;
    if (size > available)
    {
        size = available;
    }
    memcpy(mem->buf, buf, size*sizeof(char));
    mem->pos = mem->pos + size;
    return size;
}

static fpos_t seekfn(void *mst, fpos_t offset, int whence){
    size_t curr = 0;
    mst_t *mem = mst;
    switch (whence)
    {
        case SEEK_SET:
            curr = offset;
            break;
        case SEEK_CUR:
            curr = mem->pos + offset;
            break;
        case SEEK_END:
            curr = mem->size + offset;
            break;
        default:
            break;
    }
    if (curr > mem->size)
    {
        return -1;
    }
    else{
        mem->pos = curr;
        return (fpos_t)curr;
    }

}

static int closefn(void *mst){
    free(mst);
    return 0;
}

FILE *my_fmemopen(void *buf, size_t size, const char *mode){
    // mst_t *mst;
    mst_t *mst = (mst_t *)malloc(sizeof(mst_t));

    mst->size = size;
    mst->buf = buf;
    mst->pos = 0;

    return funopen(mst, readfn, writefn, seekfn, closefn);
}


int main() {
    int status;
    long i, j;
    struct tms t;
    clock_t dub;

    int tics_per_second;

    tics_per_second = sysconf(_SC_CLK_TCK);

    if (fork() == 0) {
        for (i=0, j=0; i<100000; i++) {
            FILE *fp;
            char str[] = "This is tutorialspoint.com";

            fp = fopen("file.txt", "w");
            fwrite(str, 1, sizeof(str), fp);

            fclose(fp);
        }
        exit(0);
    }

    if (wait(&status) == -1)
        perror("wait() error");
    else if (!WIFEXITED(status))
        puts("Child did not exit successfully");
    else if ((dub = times(&t)) == -1)
        perror("times() error");
    else {
        printf("process was dubbed %f seconds ago.\n\n",
               ((double) dub)/tics_per_second);
        printf("            utime           stime\n");
        printf("parent:    %f        %f\n",
               ((double) t.tms_utime)/tics_per_second,
                ((double) t.tms_stime)/tics_per_second);
        printf("child:     %f        %f\n",
               ((double) t.tms_cutime)/tics_per_second,
                ((double) t.tms_cstime)/tics_per_second);
    }

}
