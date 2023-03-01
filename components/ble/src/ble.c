#include "ble.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"

#include "gap_priv.h"

#define BLE_DEBUGGER_APP_ID 0x56

void ble_init(void){

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));

    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));

    ESP_ERROR_CHECK(esp_bluedroid_init());

    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gap_register_callback(&esp_gap_cb));

    //ESP_ERROR_CHECK(esp_ble_gattc_register_callback(esp_gattc_cb));

    ESP_ERROR_CHECK(esp_ble_gattc_app_register(BLE_DEBUGGER_APP_ID));

    //ESP_ERROR_CHECK(esp_ble_gatt_set_local_mtu(500));

    return;
}

void ble_deinit(void){ // TODO: IMPLEMENT BLE_DEINIT

}