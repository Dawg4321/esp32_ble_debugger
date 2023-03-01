#include <stdio.h>

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "cli.h"
#include "ble.h"

void app_main(void){
    ESP_ERROR_CHECK(nvs_flash_init()); // need to init nvs flash before ble and cli
    ble_init();
    cli_init();
}
