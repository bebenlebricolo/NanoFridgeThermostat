#include "thermistor_ntc_100k_3950K.h"

const thermistor_data_t thermistor_ntc_100k_3950K_data =  {
    .data = {
        {-24, 1353},
        {-22, 1193},
        {-20, 1053},
        {-17, 877},
        {-15, 778},
        {-13, 692},
        {-11, 616},
        {-8, 520},
        {-6, 465},
        {-4, 416},
        {-2, 374},
        {0, 336},
        {3, 287},
        {5, 259},
        {7, 234},
        {9, 211},
        {12, 182},
        {14, 166},
        {16, 151},
        {18, 137},
        {21, 119},
        {23, 109},
        {25, 100}
    },
    .unit = RESUNIT_KILOOHMS,
    .sample_count = THERMISTOR_NTC_100K_3950K_SAMPLE_COUNT
};
