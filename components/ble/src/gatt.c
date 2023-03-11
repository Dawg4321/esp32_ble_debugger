#include "gatt.h"
#include "gatt_priv.h"

#include <string.h>

#include "esp_log.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_gattc_api.h"

#include "mac_utils.h"

#define NUM_OF_PROFILES 1
#define GATT_APP_ID 0

static const char* TAG = "GATT_SERVICE";

struct gattc_profile_inst{
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
};

static struct gattc_profile_inst gl_profile_tab[NUM_OF_PROFILES] = {
    [GATT_APP_ID] = {
        .gattc_cb = gatt_profile_event_handler,
        .gattc_if = ESP_GATT_IF_NONE,
    },
};

esp_err_t gatt_connect_to_mac(char** mac){
    esp_bd_addr_t ble_mac;
    char_to_addr(*mac, ble_mac);
    ESP_LOGI(TAG,"Connecting to %s", *mac);
    return esp_ble_gattc_open(gl_profile_tab[GATT_APP_ID].gattc_if, ble_mac, BLE_ADDR_TYPE_PUBLIC, true);
}

void gatt_profile_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t* param){
    esp_ble_gattc_cb_param_t* p_data = (esp_ble_gattc_cb_param_t*) param;
    switch(event){
        case ESP_GATTC_REG_EVT:{
            break;
        }
        case ESP_GATTC_UNREG_EVT:{
            break;
        }
        case ESP_GATTC_CONNECT_EVT:{
            ESP_LOGI(TAG, "Connecting to device - conn_id: %d, if: %d", p_data->connect.conn_id, gattc_if);
            gl_profile_tab[GATT_APP_ID].conn_id = p_data->connect.conn_id;
            memcpy(gl_profile_tab[GATT_APP_ID].remote_bda, p_data->connect.remote_bda, sizeof(esp_bd_addr_t));
            
            esp_err_t mtu_ret = esp_ble_gattc_send_mtu_req (gattc_if, p_data->connect.conn_id);
            if (mtu_ret){
                ESP_LOGE(TAG, "config MTU error, error code = %x", mtu_ret);
            }
            break;
        }
        case ESP_GATTC_OPEN_EVT:{
            if (param->open.status != ESP_GATT_OK){
                ESP_LOGE(TAG, "Failed to open connection, status %d", p_data->open.status);
                break;
            }
            ESP_LOGI(TAG, "Successfully open connection with %02x:%02x:%02x:%02x:%02x:%02x", gl_profile_tab[GATT_APP_ID].remote_bda[0],
                                                                                             gl_profile_tab[GATT_APP_ID].remote_bda[1],
                                                                                             gl_profile_tab[GATT_APP_ID].remote_bda[2],
                                                                                             gl_profile_tab[GATT_APP_ID].remote_bda[3],
                                                                                             gl_profile_tab[GATT_APP_ID].remote_bda[4],
                                                                                             gl_profile_tab[GATT_APP_ID].remote_bda[5]);
            break;
        }
        case ESP_GATTC_DIS_SRVC_CMPL_EVT:{
            break;
        }
        case ESP_GATTC_CFG_MTU_EVT:{
            break;
        }
        case ESP_GATTC_SEARCH_RES_EVT:{
            break;
        }
        case ESP_GATTC_SEARCH_CMPL_EVT:{
            break;
        }
        case ESP_GATTC_REG_FOR_NOTIFY_EVT:{
            break;
        }
        case ESP_GATTC_NOTIFY_EVT:{
            break;
        }
        case ESP_GATTC_WRITE_DESCR_EVT:{
            break;
        }
        case ESP_GATTC_SRVC_CHG_EVT:{
            break;
        }
        case ESP_GATTC_WRITE_CHAR_EVT:{
            break;
        }
        case ESP_GATTC_DISCONNECT_EVT:{
            break;
        }
        default:{
            break;
        }
    }
    return;
}

void esp_gatt_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param){
    if(event == ESP_GATTC_REG_EVT){
        if(param->reg.status == ESP_GATT_OK){
            //gl_profile_tab[param->reg.app_id].gattc_if = gattc_if;
            gl_profile_tab[0].gattc_if = gattc_if;
        }
        else{
            ESP_LOGI(TAG, "reg app failed, app_id %04x, status %d", param->reg.app_id, param->reg.status);
            return;
        }
    }
    do{
        for(int i = 0; i < NUM_OF_PROFILES; i++){
            if(gattc_if == ESP_GATT_IF_NONE || gattc_if == gl_profile_tab[i].gattc_if) {
                if(gl_profile_tab[i].gattc_cb){
                    gl_profile_tab[i].gattc_cb(event, gattc_if, param);
                }
            }
        }
    }while(0);
}
