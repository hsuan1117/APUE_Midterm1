#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

const long long BSZ = 4096;

void start_clock(void);

void end_clock(char *msg);

static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

void start_clock() {
    st_time = times(&st_cpu);
}

void end_clock(char *msg) {
    en_time = times(&en_cpu);

    fputs(msg, stdout);
    printf("User CPU %lu, System CPU %lu, Clock Time: %lu\n",
           (clock_t) ((en_cpu.tms_utime - st_cpu.tms_utime) * 10),
           (clock_t) ((en_cpu.tms_stime - st_cpu.tms_stime) * 10),
           (clock_t) ((en_time - st_time) * 10)
    );
}

char mem_buf[(long) 2e8 + 5];
char mem_buf2[(long) 2e8 + 5];
char buf[4096 + 5];

int main() {
//    remove("output1");
//    remove("output2");
//    remove("output3");
//    remove("output4");
//    remove("output5");
//    remove("output6");

    printf("==== measured in ms ====\n");
    memset(mem_buf, 0, sizeof(mem_buf));
    printf("==== Testing [1] ====\n");
    start_clock();
    FILE *fp = fopen("source2", "rb");
    FILE *fmem = fmemopen(mem_buf, (long) 2e8, "ab");
    long long size__ = 0;
    while (fread(buf, 1, 1, fp) > 0) {
        size__++;
        fwrite(buf, 1, 1, fmem);
    }
    end_clock("[1]: ");
    FILE *output_file = fopen("output1", "wb");
    fwrite(mem_buf, 1, size__, output_file);
    fclose(output_file);

    memset(mem_buf, 0, sizeof(mem_buf));
    printf("==== Testing [2] ====\n");
    start_clock();
    fp = fopen("source2", "rb");
    fmem = fmemopen(mem_buf, (long) 2e8, "ab");
    size__ = 0;
    while (fread(buf, 32, 1, fp) > 0) {
        size__++;
        fwrite(buf, 32, 1, fmem);
    }
    end_clock("[2]: ");
    output_file = fopen("output2", "wb");
    fwrite(mem_buf, 1, size__, output_file);
    fclose(output_file);

    memset(mem_buf, 0, sizeof(mem_buf));
    printf("==== Testing [3] ====\n");
    start_clock();
    fp = fopen("source2", "rb");
    fmem = fmemopen(mem_buf, (long) 2e8, "ab");
    size__ = 0;
    while (fread(buf, 1024, 1, fp) > 0) {
        size__++;
        fwrite(buf, 1024, 1, fmem);
    }
    end_clock("[3]: ");
    output_file = fopen("output3", "wb");
    fwrite(mem_buf, 1, size__, output_file);
    fclose(output_file);

    memset(mem_buf, 0, sizeof(mem_buf));
    printf("==== Testing [4] ====\n");
    start_clock();
    fp = fopen("source2", "rb");
    fmem = fmemopen(mem_buf, (long) 2e8, "ab");
    size__ = 0;
    while (fread(buf, 4096, 1, fp) > 0) {
        size__++;
        fwrite(buf, 4096, 1, fmem);
    }
    end_clock("[4]: ");
    output_file = fopen("output4", "wb");
    fwrite(mem_buf, 1, size__, output_file);
    fclose(output_file);

    // using fgets fputs
    memset(mem_buf, 0, sizeof(mem_buf));

    printf("==== Testing [5] ====\n");
    start_clock();
    fp = fopen("source2", "rb");
    fmem = fmemopen(mem_buf, (long) 2e8, "ab");
    size__ = 0;
    while (fgets(buf, 4096, fp) != NULL) {
        size__++;
        fputs(buf, fmem);
    }
    end_clock("[5]: ");
    output_file = fopen("output5", "wb");
    fwrite(mem_buf, 1, size__, output_file);
    fclose(output_file);
    memset(mem_buf, 0, sizeof(mem_buf));

    printf("==== Testing [6] ====\n");
    start_clock();
    fp = fopen("source2", "rb");
    fmem = fmemopen(mem_buf, (long) 2e6, "ab");
    size__ = 0;
    while (fgets(mem_buf2, (long) 2e8, fp) != NULL) {
        size__++;
        fputs(mem_buf2, fmem);
    }
    end_clock("[6]: ");
    output_file = fopen("output6", "wb");
    fwrite(mem_buf, 1, size__, output_file);
    fclose(output_file);

}
