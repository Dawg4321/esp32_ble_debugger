#include "mac_utils.h"

#include <string.h>
#include <stdio.h>

#include "esp_log.h"

#define MAC_ADDR_LAYOUT "%02x:%02x:%02x:%02x:%02x:%02x"

static const char* TAG = "BLE_UTILS";

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

void print_gatt_service_info(esp_gattc_service_elem_t service, unsigned int number_to_print){
    ESP_LOGI(TAG, "~~~Service %u~~~", number_to_print);
    if(service.uuid.len == ESP_UUID_LEN_16){
        ESP_LOGI(TAG, "UUID: 0x%04x", (unsigned int)service.uuid.uuid.uuid16);
    }
    else if(service.uuid.len == ESP_UUID_LEN_32){
        ESP_LOGI(TAG, "UUID: 0x%04x", (unsigned int)service.uuid.uuid.uuid32);
    }
    else{
        ESP_LOGI(TAG, "UUID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                    service.uuid.uuid.uuid128[0],
                    service.uuid.uuid.uuid128[1],
                    service.uuid.uuid.uuid128[2],
                    service.uuid.uuid.uuid128[3],
                    service.uuid.uuid.uuid128[4],
                    service.uuid.uuid.uuid128[5],
                    service.uuid.uuid.uuid128[6],
                    service.uuid.uuid.uuid128[7],
                    service.uuid.uuid.uuid128[8],
                    service.uuid.uuid.uuid128[9],
                    service.uuid.uuid.uuid128[10],
                    service.uuid.uuid.uuid128[11],
                    service.uuid.uuid.uuid128[12],
                    service.uuid.uuid.uuid128[13],
                    service.uuid.uuid.uuid128[14],
                    service.uuid.uuid.uuid128[15]);
    }
    if(!service.is_primary){
        ESP_LOGI(TAG, "TEST");
    }
    ESP_LOGI(TAG, "Primary Service: %c", (service.is_primary ? 'y' : 'n'));
    return;
}