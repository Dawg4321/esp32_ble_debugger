#ifndef MAC_UTILS_H
#define MAC_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#define MAC_ADDR_LENGTH 6

bool char_to_uint_mac(char* mac_str, uint8_t* uint_mac);

#endif