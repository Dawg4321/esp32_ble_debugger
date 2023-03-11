#include "gap.h"
#include "gap_priv.h"

#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include "esp_log.h"
#include "esp_bt_defs.h"

#include "mac_utils.h"

static int gap_scan_duration = 0;

static const char* TAG = "GAP_SERVICE";

typedef struct {
    esp_bd_addr_t mac;
    bool mac_set_flag;
    uint8_t* name;
    uint8_t name_size;
    bool name_alloc_flag;
}scan_target_t;

static scan_target_t scan_target = {
    .mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // invalid mac
    .mac_set_flag = false,
    .name = NULL,
    .name_size = 0,
    .name_alloc_flag = false
};

void gap_set_scan_duration(int duration){
    gap_scan_duration = duration;
    return;
}

esp_err_t gap_set_scan_params(esp_ble_scan_params_t ble_scan_params){
    return esp_ble_gap_set_scan_params(&ble_scan_params);
    
}

void gap_reset_scan_target(){
    if(scan_target.mac_set_flag){
        scan_target.mac[0] = 0x00;
        scan_target.mac[1] = 0x00;
        scan_target.mac[2] = 0x00;
        scan_target.mac[3] = 0x00;
        scan_target.mac[4] = 0x00;
        scan_target.mac[5] = 0x00;
        scan_target.mac_set_flag = false;
    }
    
    if(scan_target.name_alloc_flag){
        free(scan_target.name);
        scan_target.name = NULL;
        scan_target.name_size = 0;
        scan_target.name_alloc_flag = false;
    }
    return;
}

bool gap_set_target_name(char** target_name){
    scan_target.name = malloc((strlen(*target_name))*sizeof(uint8_t)); // not adding one for null terminator as gap_scan names are not null terminated
    if(scan_target.name != NULL){
        memcpy(scan_target.name, *target_name, strlen(*target_name));
        scan_target.name_size = strlen(*target_name);
        scan_target.name_alloc_flag = true;
        return true;
    }
    return false;
}

bool gap_set_target_mac(char** target_mac){
    ESP_LOGI(TAG, "%s", *target_mac);
    scan_target.mac_set_flag = char_to_addr(*target_mac, scan_target.mac);
    return scan_target.mac_set_flag;
}

static bool gap_scan_compare(esp_bd_addr_t mac, uint8_t* name, size_t name_size){
    bool mac_match, name_match = false;
    if((scan_target.mac_set_flag && memcmp(scan_target.mac, mac, ESP_BD_ADDR_LEN) == 0) 
      || (!scan_target.mac_set_flag))
    {
        mac_match = true; // mac addr has been set and matches target mac or mac addr is not set (no need to compare)
    }
    else{
        mac_match = false;
    }
    
    if((scan_target.name_alloc_flag && scan_target.name_size == name_size &&memcmp(scan_target.name, name, name_size) == 0)
      || (!scan_target.name_alloc_flag))
    {
        name_match = true; // name has been set and matches target name or name is not set (no need to compare)
    }
    else{
        name_match = false;
    }

    return (mac_match && name_match);
}

void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param){
    switch(event){
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:{
            esp_ble_gap_start_scanning(gap_scan_duration);
            break;
        }
        case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:{
            // occurs when scan start succeeds or fails
            ESP_LOGI(TAG,"Starting GAP scan...\n");
            if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(TAG, "Failed to start GAP scan: error status = %x", param->scan_start_cmpl.status);
                break;
            }
            break;
        }
        case ESP_GAP_BLE_SCAN_RESULT_EVT:{
            esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
            switch (scan_result->scan_rst.search_evt){
                case ESP_GAP_SEARCH_INQ_RES_EVT:
                    uint8_t adv_name_len = 0;
                    uint8_t *adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
                    if(gap_scan_compare(scan_result->scan_rst.bda, adv_name, adv_name_len)){ // if scan result matches set target
                        if(adv_name_len != 0){
                            esp_log_buffer_char(TAG, adv_name, adv_name_len);
                        }
                        else{
                            ESP_LOGI(TAG, "No Adv Name (adv name length = 0)");
                        }
                        esp_log_buffer_hex(TAG, scan_result->scan_rst.bda, 6);
                        ESP_LOGI(TAG, "Searched Adv Data Length: %d, Scan Response Length: %d\n", scan_result->scan_rst.adv_data_len, scan_result->scan_rst.scan_rsp_len);
                    }
                    break;
                case ESP_GAP_SEARCH_INQ_CMPL_EVT:
                    break;
                default:
                    break;
            }
            break;
        }
        case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:{
            if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
                ESP_LOGE(TAG, "Failed to stop scan: error status = %x", param->scan_stop_cmpl.status);
                break;
            }
            ESP_LOGI(TAG, "GAP scan stopped!");
            break;
        }
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:{
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
                ESP_LOGE(TAG, "Failed to stop adv: error status = %x", param->adv_stop_cmpl.status);
                break;
            }
            ESP_LOGI(TAG, "Successfully stopped adv!");
            break;
        }
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:{
            ESP_LOGI(TAG, "Updated connection params: status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                    param->update_conn_params.status,
                    param->update_conn_params.min_int,
                    param->update_conn_params.max_int,
                    param->update_conn_params.conn_int,
                    param->update_conn_params.latency,
                    param->update_conn_params.timeout);
            break;
        }
        default:{
            break;
        }
    }
    return;
}