#ifndef MAC_UTILS_H
#define MAC_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include "esp_bt_defs.h"

bool char_to_addr(char* mac_str, esp_bd_addr_t uint_mac);

#endif