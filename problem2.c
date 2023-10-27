#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BSZ 32

int main() {
    struct tms t;
    int tics_per_second = sysconf(_SC_CLK_TCK);
    double u_start = (double) t.tms_utime / tics_per_second;
    double s_start = (double) t.tms_stime / tics_per_second;
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

    printf("User CPU: %f\n", (double) t.tms_utime / tics_per_second - u_start);
    printf("System CPU: %f\n", (double) t.tms_stime / tics_per_second - s_start);
}
