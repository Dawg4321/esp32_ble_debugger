#ifndef GAP_H
#define GAP_H

#include "esp_gap_ble_api.h"

void gap_set_scan_duration(int duration);
void gap_set_scan_params(esp_ble_scan_params_t ble_scan_params);

#endif