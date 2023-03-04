#include "commands.h"

#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gap.h"

#define BLE_SCAN_ARG_NUM 1
#define DEFAULT_BLE_SCAN_DURATION_S 5 // seconds

static struct{
    struct arg_int* duration;
    struct arg_str* adv_name;
    struct arg_str* mac_addr;
    struct arg_end* end;
}ble_scan_args;

int ble_scan_arg_parse(int argc, char** argv){
    int errors = arg_parse(argc, argv, (void**)&ble_scan_args);
    if (errors > 0){
        arg_print_errors(stdout, ble_scan_args.end, "scan");
        printf("Try 'help' for more information.\n");
        return 0;
    }

    if(ble_scan_args.adv_name->count >= 0){
        gap_set_target_name((char**)ble_scan_args.adv_name->sval);
    }
    if(ble_scan_args.mac_addr->count >= 0){
        gap_set_target_mac((char**)ble_scan_args.mac_addr->sval);
    }

    int scan_duration = 0;
    if(ble_scan_args.duration->count == 0){ // no duration flag thus use default scan duration
        scan_duration = DEFAULT_BLE_SCAN_DURATION_S;
    }
    else{
        scan_duration = ble_scan_args.duration->ival[0];
    }
    gap_set_scan_duration(scan_duration);

    esp_ble_scan_params_t ble_scan_params = {
        .scan_type              = BLE_SCAN_TYPE_ACTIVE,
        .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval          = 0x50,
        .scan_window            = 0x30,
        .scan_duplicate         = BLE_SCAN_DUPLICATE_ENABLE
    };
    gap_set_scan_params(ble_scan_params); // ble scan will start once parameters are successfully set
    vTaskDelay((1000*scan_duration)/portTICK_PERIOD_MS);
    gap_reset_scan_target();
    return 0;
}

static void register_ble_scan(void){
    ble_scan_args.duration = arg_intn("t", "time", "<1-10>", 0, 1,"OPTIONAL: Scan time - s");
    ble_scan_args.adv_name = arg_strn("n", "name", "<name>", 0, 1,"OPTIONAL: Name to scan for");
    ble_scan_args.mac_addr = arg_strn("m", "mac", "<XX:XX:XX:XX:XX:XX>", 0, 1,"OPTIONAL: MAC Address to scan for");
    ble_scan_args.end = arg_end(BLE_SCAN_ARG_NUM);

    const esp_console_cmd_t ble_scan = {
        .command = "scan",
        .help = "Scans for BLE devices",
        .hint = NULL,
        .func = &ble_scan_arg_parse,
        .argtable = &ble_scan_args,
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&ble_scan));
    return;
}

static void ble_scan_deinit(void){
    arg_freetable((void**)&ble_scan_args, sizeof(ble_scan_args) / sizeof(ble_scan_args.duration));
    return;
}

void cli_commands_init(void){
    esp_console_register_help_command();
    register_ble_scan();

    return;
}

void cli_commands_deinit(void){
    ble_scan_deinit();
    return;
}