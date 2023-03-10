#include "mac_utils.h"

#include <string.h>
#include <stdio.h>

#define MAC_ADDR_LAYOUT "%02x:%02x:%02x:%02x:%02x:%02x"

bool char_to_addr(char* mac_str, esp_bd_addr_t mac){
    unsigned int val_0, val_1, val_2, val_3, val_4, val_5 = 0;
    if(sscanf(mac_str, MAC_ADDR_LAYOUT, &val_0, &val_1, &val_2, &val_3, &val_4, &val_5) == ESP_BD_ADDR_LEN){
        mac[0] = val_0;
        mac[1] = val_1;
        mac[2] = val_2;
        mac[3] = val_3;
        mac[4] = val_4;
        mac[5] = val_5;
        return true;
    }
    return false;
}
