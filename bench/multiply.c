// *************************************************************************
// multiply filter bencmark
// -------------------------------------------------------------------------
//
// This benchmark tests the software multiply implemenation.

unsigned int input1_multiply[100];
unsigned int input2_multiply[100];
unsigned int results_multiply[100];

void multiply_data_init(unsigned int nonce) {
    unsigned int i;
    unsigned int x = nonce;
    unsigned int y = nonce;

    for (i = 0; i < 100; i++) {
        x = (x * 97 + 17) % 10009;
        y = (y * 17 + 23) % 10007;
        input1_multiply[i] = x;
        input2_multiply[i] = y;
    }
}

unsigned int multiply(unsigned int x, unsigned int y) {
    unsigned int i;
    unsigned int result = 0;

    for (i = 0; i < 32; i++) {
        if ((x & 0x1) == 1)
            result = result + y;

        x = x >> 1;
        y = y << 1;
    }

    return result;
}

int verify_multiply(unsigned int n, unsigned int* test) {
    unsigned int i;
    unsigned int result = 0;

    for (i = 0; i < n; i++) {
        unsigned int t0 = input1_multiply[i];
        unsigned int t1 = input2_multiply[i];
        unsigned int v = results_multiply[i];
        if (t0 * t1 != v)
            return 1;

        result += v;
    }

    return (int)result;
}

int run_multiply(int dummy_0, int nonce) {
    unsigned int i;

    multiply_data_init((unsigned int)nonce);
    for (i = 0; i < 100; i++) {
        results_multiply[i] = multiply(input1_multiply[i], input2_multiply[i]);
    }

    return verify_multiply(100, results_multiply);
}
