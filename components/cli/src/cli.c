#include "cli.h"

#include  "esp_console.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "commands.h"

void cli_init(){

    ESP_ERROR_CHECK(nvs_flash_init());

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_cfg = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_cfg.prompt = "esp32 >";
    esp_console_dev_uart_config_t uart_cfg = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_cfg, &repl_cfg, &repl));

    cli_commands_init();

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    return;
}  

void cli_deinit(){
    return;
}