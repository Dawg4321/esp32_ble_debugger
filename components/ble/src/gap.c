#include "gap.h"
#include "gap_priv.h"

#include "esp_log.h"

static int gap_scan_duration = 10;

static const char* TAG = "GAP_SERVICE";

void gap_set_scan_duration(int duration){
    gap_scan_duration = duration;
    return;
}

void gap_set_scan_params(esp_ble_scan_params_t ble_scan_params){
    esp_ble_gap_set_scan_params(&ble_scan_params);
    return;
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
                    uint8_t *adv_name = NULL;
                    uint8_t adv_name_len = 0;
                    
                    if(adv_name_len != 0){
                        adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
                        esp_log_buffer_char(TAG, adv_name, adv_name_len);
                    }
                    else{
                        ESP_LOGI(TAG, "No Adv Name (adv name length = 0)");
                    }
                    esp_log_buffer_hex(TAG, scan_result->scan_rst.bda, 6);
                    ESP_LOGI(TAG, "searched Adv Data Length: %d, Scan Response Length: %d\n", scan_result->scan_rst.adv_data_len, scan_result->scan_rst.scan_rsp_len);
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