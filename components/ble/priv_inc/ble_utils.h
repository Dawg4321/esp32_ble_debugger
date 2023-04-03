#ifndef BLE_UTILS_H
#define BLE_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include "esp_bt_defs.h"
#include "esp_gatt_defs.h"

bool char_to_addr(char* mac_str, esp_bd_addr_t uint_mac);
void print_gatt_service_info(esp_gattc_service_elem_t service, unsigned int number_to_print);

#endif