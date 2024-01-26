#include "thermistor_ntc_100k_3950K.h"

const thermistor_data_t thermistor_ntc_100k_3950K_data =  {
    .data = {
        {-24, 1353},
        {-21, 1121},
        {-19, 991},
        {-17, 877},
        {-15, 778},
        {-12, 653},
        {-10, 582},
        {-8, 520},
        {-6, 465},
        {-3, 394},
        {-1, 354},
        {0, 336},
        {2, 302},
        {4, 272},
        {7, 234},
        {9, 211},
        {11, 192},
        {13, 174},
        {16, 151},
        {18, 137},
        {20, 125},
        {22, 114},
        {24, 104}
    },
    .unit = RESUNIT_KILOOHMS,
    .sample_count = THERMISTOR_NTC_100K_3950K_SAMPLE_COUNT
};
