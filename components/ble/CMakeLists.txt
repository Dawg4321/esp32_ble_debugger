set(BLE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(BLE_SRCS ${BLE_DIR}/src/ble.c
             ${BLE_DIR}/src/gap.c)

idf_component_register(SRCS ${BLE_SRCS}
                       INCLUDE_DIRS ${BLE_DIR}/inc
                       PRIV_INCLUDE_DIRS ${BLE_DIR}/priv_inc
                       REQUIRES bt)