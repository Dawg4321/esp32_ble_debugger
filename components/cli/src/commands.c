#include "commands.h"


#include "esp_console.h"
#include "argtable3/argtable3.h"

#define BLE_SCAN_ARG_NUM 1
#define MINIMUM_BLE_SCAN_TIME_S 1 // seconds
#define MAXIMUM_BLE_SCAN_TIME_S 1 // seconds
#define DEFAULT_BLE_SCAN_DURATION_S 5 // seconds

static struct{
    struct arg_int* duration;
    struct arg_end* end;
}ble_scan_args;

int scan_arg_parse(int argc, char** argv){
    int scan_duration = 0;
    arg_parse(argc, argv, (void**)&ble_scan_args);
    
    if(ble_scan_args.duration->count == 0){
        scan_duration = DEFAULT_BLE_SCAN_DURATION_S;
    }
    else{
        scan_duration = ble_scan_args.duration->ival[0];
    }

    return 0;
}

static void register_ble_scan(void){
    ble_scan_args.duration = arg_intn("t", "time", "<1-10>", MINIMUM_BLE_SCAN_TIME_S, MAXIMUM_BLE_SCAN_TIME_S,"Scan time - s");
    ble_scan_args.end = arg_end(BLE_SCAN_ARG_NUM);

    const esp_console_cmd_t ble_scan = {
        .command = "scan",
        .help = "Scans for BLE devices",
        .hint = NULL,
        .func = &scan_arg_parse,
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