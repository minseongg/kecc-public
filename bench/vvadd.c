//**************************************************************************
// Vector-vector add benchmark
//--------------------------------------------------------------------------
//
// This benchmark uses adds to vectors and writes the results to a
// third vector.

unsigned input1_vvadd[1000];
unsigned input2_vvadd[1000];
unsigned results_vvadd[1000];

void vvadd_init(unsigned nonce) {
    unsigned i;
    unsigned x = nonce;
    unsigned y = nonce;

    for (i = 0; i < 1000; i++) {
        x = (x * 97u + 17u) % 1009u;
        y = (x * 17u + 23u) % 1007u;
        input1_vvadd[i] = x;
        input2_vvadd[i] = y;
    }
}

void vvadd(unsigned n, unsigned a[1000], unsigned b[1000], unsigned c[1000]) {
    unsigned i;

    for (i = 0; i < n; i++)
        c[i] = a[i] + b[i];
}

int verify_vvadd(unsigned n, unsigned* test) {
    unsigned i;
    unsigned result = 0;

    for (i = 0; i < n; i++) {
        unsigned v = test[i];
        result += v;
    }

    return result;
}

int run_vvadd(int dummy_0, int nonce) {
    vvadd_init(nonce);
    vvadd(1000, input1_vvadd, input2_vvadd, results_vvadd);
    return verify_vvadd(1000, results_vvadd);
}
