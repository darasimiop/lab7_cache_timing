#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KB(x) ((x) * 1024)
#define ITERATIONS 100000000

// Test region sizes in KB
int test_sizes[] = {
    2, 4, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 512, 600, 768,
    1024, 1536, 2048, 3072, 4096, 5120, 6144, 8192, 10240, 12288,
    16384, 24567, 32768, 65536, 98304, 131072, 262144, 393216, 524288, 1048576
};

// High-resolution clock
uint64_t now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec) * 1e9 + ts.tv_nsec;
}

// Create pointer-chasing pattern
void fill_pattern(uint64_t *A, int elements, int stride) {
    for (int i = 0; i < elements - 1; ++i) {
        A[i * stride] = (i + 1) * stride;
    }
    A[(elements - 1) * stride] = 0;
}

// Latency test for a given memory region size
double run_latency_test(int region_kb, int stride_bytes) {
    int stride = stride_bytes / sizeof(uint64_t);
    int total_bytes = KB(region_kb);
    int elements = total_bytes / stride_bytes;

    uint64_t *A = (uint64_t *)malloc(total_bytes);
    if (!A) {
        fprintf(stderr, "❌ Memory allocation failed for %d KB\n", region_kb);
        return -1;
    }

    memset(A, 0, total_bytes);
    fill_pattern(A, elements, stride);

    int current = 0;
    volatile int sum = 0;

    uint64_t start = now_ns();
    for (uint64_t i = 0; i < ITERATIONS; ++i) {
        current = A[current];
        sum += current;
    }
    uint64_t end = now_ns();

    free(A);

    double latency_ns = (double)(end - start) / ITERATIONS;
    return latency_ns;
}

int main() {
    printf("Region_KB,Latency_ns\n");

    for (int i = 0; i < sizeof(test_sizes)/sizeof(test_sizes[0]); ++i) {
        int region_kb = test_sizes[i];
        double latency = run_latency_test(region_kb, 64);
        printf("%d,%.2f\n", region_kb, latency);
    }

    return 0;
}
