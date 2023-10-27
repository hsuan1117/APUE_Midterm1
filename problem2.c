#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BSZ 1024

int main() {
    struct tms t;
    int tics_per_second = sysconf(_SC_CLK_TCK);

    char buf[BSZ];
    memset(buf, 0, BSZ);

    remove("output1");

    FILE *fp = fopen("source2", "r");
    while (fread(buf, BSZ, 1, fp) > 0) {
//        printf("hi");
        FILE *fp2 = fopen("output1", "a");
        fwrite(buf, BSZ, 1, fp2);
        fclose(fp2);
    }

    printf("            utime           stime\n");
    printf("parent:    %f        %f\n",
           ((double) t.tms_utime) / tics_per_second,
           ((double) t.tms_stime) / tics_per_second);
    printf("child:     %f        %f\n",
           ((double) t.tms_cutime) / tics_per_second,
           ((double) t.tms_cstime) / tics_per_second);
}
