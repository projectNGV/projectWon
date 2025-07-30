#include "asclin_ecual.h"

const Asclin_ConfigType asclinConfig[2] = {
    [0] = {
            .module = &MOUDLE_ASCLIN0,
            .baudRate = ASCLIN_BAUD_RATE_115200,
            .txPin = {&MODULE_P14, 0},
            .rxPin = {&MODULE_P14, 1},
          },
    [1] = {
        .module = &MOUDLE_ASCLIN0,
        .baudRate = ASCLIN_BAUD_RATE_115200,
        .txPin = {&MODULE_P15, 0},
        .rxPin = {&MODULE_P15, 1}
    },
};

