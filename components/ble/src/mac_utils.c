#include "mac_utils.h"

#include <string.h>
#include <stdio.h>

#define MAC_ADDR_LAYOUT "%02x:%02x:%02x:%02x:%02x:%02x"

bool char_to_uint_mac(char* mac_str, uint8_t* uint_mac){
    unsigned int val_0, val_1, val_2, val_3, val_4, val_5 = 0;
    if(sscanf(mac_str, MAC_ADDR_LAYOUT, &val_0, &val_1, &val_2, &val_3, &val_4, &val_5) == MAC_ADDR_LENGTH){
        uint_mac[0] = val_0;
        uint_mac[1] = val_1;
        uint_mac[2] = val_2;
        uint_mac[3] = val_3;
        uint_mac[4] = val_4;
        uint_mac[5] = val_5;
        return true;
    }
    return false;
}
