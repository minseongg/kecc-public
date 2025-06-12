//**************************************************************************
// Radix Sort benchmark
//--------------------------------------------------------------------------
//
// This benchmark uses radix sort to sort an array of integers. The
// implementation is largely adapted from Numerical Recipes for C.

unsigned int input_rsort_data[2048];

void rsort_data_init(unsigned int nonce) {
    int i;
    unsigned int x = nonce;

    for (i = 0; i < 2048; i++) {
        x = (x * 97u + 17u) % 10000007u;
        input_rsort_data[i] = x;
    }
}

unsigned int fetch_add(unsigned int* ptr, unsigned int inc) {
    return (*ptr += inc) - inc;
}

void memcpy_int(unsigned int* dest, unsigned int* src, int size) {
    int i;

    for (i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

unsigned int bucket[256];

void rsort(int n, unsigned int* arrIn, unsigned int* scratchIn) {
    int log_exp = 0;
    unsigned int *arr = arrIn, *scratch = scratchIn;
    int p;
    int b;

    while (log_exp < 8 * sizeof(unsigned int)) {
        for (b = 0; b < (1 << 8); b++)
            bucket[b] = 0;

        for (p = 0; p < n - 3; p += 4) {
            unsigned int a0 = arr[p + 0];
            unsigned int a1 = arr[p + 1];
            unsigned int a2 = arr[p + 2];
            unsigned int a3 = arr[p + 3];
            fetch_add(&bucket[(a0 >> log_exp) % (1 << 8)], 1);
            fetch_add(&bucket[(a1 >> log_exp) % (1 << 8)], 1);
            fetch_add(&bucket[(a2 >> log_exp) % (1 << 8)], 1);
            fetch_add(&bucket[(a3 >> log_exp) % (1 << 8)], 1);
        }
        for (; p < n; p++)
            bucket[(arr[p] >> log_exp) % (1 << 8)]++;

        unsigned int prev = bucket[0];
        prev += fetch_add(&bucket[1], prev);
        for (b = 2; b < (1 << 8); b += 2) {
            prev += fetch_add(&bucket[b + 0], prev);
            prev += fetch_add(&bucket[b + 1], prev);
        }

        for (p = n - 1; p >= 3; p -= 4) {
            unsigned int a0 = arr[p - 0];
            unsigned int a1 = arr[p - 1];
            unsigned int a2 = arr[p - 2];
            unsigned int a3 = arr[p - 3];
            unsigned int* pb0 = &bucket[(a0 >> log_exp) % (1 << 8)];
            unsigned int* pb1 = &bucket[(a1 >> log_exp) % (1 << 8)];
            unsigned int* pb2 = &bucket[(a2 >> log_exp) % (1 << 8)];
            unsigned int* pb3 = &bucket[(a3 >> log_exp) % (1 << 8)];
            unsigned int s0 = fetch_add(pb0, -1);
            unsigned int s1 = fetch_add(pb1, -1);
            unsigned int s2 = fetch_add(pb2, -1);
            unsigned int s3 = fetch_add(pb3, -1);
            scratch[s0 - 1] = a0;
            scratch[s1 - 1] = a1;
            scratch[s2 - 1] = a2;
            scratch[s3 - 1] = a3;
        }
        for (; p >= 0; p--)
            scratch[--bucket[(arr[p] >> log_exp) % (1 << 8)]] = arr[p];

        unsigned int* tmp = arr;
        arr = scratch;
        scratch = tmp;

        log_exp += 8;
    }
    if (arr != arrIn)
        memcpy_int(arr, scratch, n);
}

int verify_rsort(int n, unsigned int* test) {
    int i;
    unsigned int result = 0;

    for (i = 0; i < n - 1; i++) {
        unsigned int t0 = test[i], t1 = test[i + 1];
        if (t0 > t1)
            return 1;

        result += t0;
    }

    return result;
}

unsigned int scratch[2048];

int run_rsort(int dummy_0, int nonce) {
    rsort_data_init((unsigned int)nonce);
    rsort(2048, input_rsort_data, scratch);

    return verify_rsort(2048, input_rsort_data);
}
