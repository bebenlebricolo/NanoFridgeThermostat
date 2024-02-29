#include "mcu_time.h"

void time_default(mcu_time_t * time)
{
    time->milliseconds = 0;
    time->seconds = 0;
}