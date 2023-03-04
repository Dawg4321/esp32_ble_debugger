#ifndef GAP_H
#define GAP_H

#include "esp_gap_ble_api.h"

void gap_set_scan_duration(int duration);
void gap_set_scan_params(esp_ble_scan_params_t ble_scan_params);
void gap_reset_scan_target(void);
bool gap_set_target_name(char** target_name);
bool gap_set_target_mac(char** target_mac);

#endif