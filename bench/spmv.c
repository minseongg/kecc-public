//**************************************************************************
// Double-precision sparse matrix-vector multiplication benchmark
//--------------------------------------------------------------------------

double val[2399];
int idx[2399];

double x[500];
int ptr[501] = {
    0, 4, 8, 10, 15, 22, 29, 33, 34, 36, 39, 44, 44, 47, 55, 61, 66, 68, 75, 82, 86, 91, 98, 104, 109, 113, 126, 131, 134,
    136, 143, 153, 159, 168, 170, 174, 176, 180, 187, 192, 198, 200, 204, 211, 213, 222, 225, 228, 233, 241, 247, 253, 256,
    262, 264, 270, 274, 277, 283, 288, 294, 301, 307, 312, 315, 319, 325, 329, 332, 338, 340, 342, 348, 353, 359, 362, 366,
    368, 374, 387, 393, 400, 403, 406, 412, 420, 425, 428, 433, 441, 442, 448, 456, 462, 468, 469, 477, 479, 485, 490, 495,
    501, 508, 513, 518, 524, 532, 535, 538, 542, 546, 552, 561, 563, 567, 571, 574, 576, 581, 584, 588, 592, 593, 597, 599,
    607, 612, 615, 619, 626, 633, 640, 646, 650, 653, 659, 665, 670, 673, 678, 685, 691, 697, 703, 708, 712, 717, 719, 722,
    725, 732, 736, 738, 742, 747, 753, 757, 763, 767, 769, 769, 777, 780, 784, 790, 797, 807, 810, 813, 815, 817, 822, 829,
    830, 835, 844, 848, 851, 854, 862, 864, 873, 876, 881, 884, 888, 895, 899, 901, 904, 908, 912, 919, 927, 931, 937, 941,
    944, 948, 950, 955, 960, 963, 970, 975, 979, 983, 985, 992, 993, 998, 1001, 1008, 1012, 1014, 1018, 1023, 1030, 1036,
    1040, 1045, 1052, 1058, 1061, 1066, 1069, 1071, 1077, 1080, 1084, 1087, 1090, 1092, 1095, 1100, 1102, 1110, 1117, 1122,
    1130, 1133, 1138, 1142, 1144, 1146, 1153, 1158, 1160, 1167, 1172, 1176, 1179, 1183, 1188, 1191, 1193, 1196, 1204, 1210,
    1216, 1219, 1221, 1225, 1229, 1234, 1236, 1239, 1244, 1248, 1253, 1256, 1264, 1268, 1271, 1272, 1275, 1278, 1284, 1287,
    1291, 1296, 1300, 1304, 1306, 1313, 1317, 1322, 1326, 1332, 1336, 1344, 1349, 1356, 1363, 1368, 1369, 1378, 1381, 1385,
    1388, 1395, 1399, 1407, 1414, 1422, 1425, 1433, 1435, 1438, 1439, 1445, 1448, 1450, 1453, 1456, 1470, 1472, 1475, 1481,
    1487, 1491, 1494, 1497, 1498, 1503, 1507, 1513, 1517, 1524, 1529, 1534, 1542, 1547, 1550, 1550, 1552, 1553, 1556, 1559,
    1563, 1571, 1577, 1580, 1585, 1590, 1594, 1598, 1600, 1602, 1610, 1615, 1620, 1628, 1634, 1637, 1649, 1652, 1656, 1662,
    1664, 1670, 1674, 1678, 1687, 1697, 1704, 1709, 1715, 1720, 1725, 1728, 1736, 1740, 1747, 1750, 1754, 1760, 1763, 1765,
    1773, 1781, 1783, 1788, 1795, 1802, 1810, 1815, 1820, 1824, 1829, 1836, 1839, 1843, 1847, 1849, 1854, 1859, 1863, 1873,
    1880, 1882, 1891, 1895, 1899, 1904, 1909, 1914, 1919, 1923, 1927, 1932, 1938, 1943, 1949, 1954, 1960, 1965, 1968, 1974,
    1980, 1983, 1990, 1992, 1995, 2002, 2011, 2017, 2024, 2028, 2035, 2037, 2048, 2055, 2063, 2067, 2069, 2078, 2081, 2085,
    2086, 2090, 2097, 2101, 2107, 2110, 2112, 2116, 2119, 2122, 2129, 2136, 2143, 2146, 2156, 2162, 2171, 2175, 2179, 2186,
    2189, 2194, 2198, 2206, 2211, 2215, 2222, 2228, 2237, 2241, 2245, 2256, 2259, 2269, 2272, 2275, 2278, 2279, 2281, 2287,
    2292, 2297, 2304, 2311, 2316, 2319, 2322, 2327, 2333, 2340, 2343, 2345, 2350, 2358, 2365, 2365, 2368, 2373, 2379, 2388,
    2394, 2399};

void spmv_init(int nonce) {
    int i;
    int t1 = nonce;
    int t2 = nonce;

    for (i = 0; i < 2399; i++) {
        t1 = (t1 * 97 + 17) % 1000;
        t2 = (t2 * 17 + 23) % 500;
        val[i] = (double) t1;
        idx[i] = t2;
    }

    for (i = 0; i < 500; i++) {
        t1 = (t1 * 17 + 23) % 1000;
        x[i] = (double) t1;
    }
}
    
void spmv(int r, double* val, int* idx, double* x, int* ptr, double* y) {
    int i;

    for (i = 0; i < r; i++) {
        int k;
        double yi0 = 0, yi1 = 0, yi2 = 0, yi3 = 0;
        for (k = ptr[i]; k < ptr[i + 1] - 3; k += 4) {
            yi0 += val[k + 0] * x[idx[k + 0]];
            yi1 += val[k + 1] * x[idx[k + 1]];
            yi2 += val[k + 2] * x[idx[k + 2]];
            yi3 += val[k + 3] * x[idx[k + 3]];
        }
        for (; k < ptr[i + 1]; k++) {
            yi0 += val[k] * x[idx[k]];
        }
        y[i] = (yi0 + yi1) + (yi2 + yi3);
    }
}

int verifyDouble(int n, double* test) {
    int i;
    int result = 0;

    for (i = 0; i < n; i++) {
        result ^= (int) (test[i]);
    }

    return result;
}

double y[500];

int run_spmv(int dummy_0, int nonce) {
    spmv_init(nonce);
    spmv(500, val, idx, x, ptr, y);

    return verifyDouble(500, y);
}
