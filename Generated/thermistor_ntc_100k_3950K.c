#include "thermistor_ntc_100k_3950K.h"

const thermistor_data_t thermistor_ntc_100k_3950K_data =  {
    .data = {
        {-24, 1353},
        {-19, 991},
        {-14, 734},
        {-9, 550},
        {-4, 416},
        {1, 318},
        {6, 246},
        {11, 192},
        {16, 151},
        {21, 119}
    },
    .unit = RESUNIT_KILOOHMS,
    .sample_count = THERMISTOR_NTC_100K_3950K_SAMPLE_COUNT
};
