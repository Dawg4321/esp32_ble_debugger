#ifndef GATT_PRIV_H
#define GATT_PRIV_H

#include "esp_gattc_api.h"

void gatt_profile_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
void esp_gatt_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

#endif