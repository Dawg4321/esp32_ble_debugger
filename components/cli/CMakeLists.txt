
set(CLI_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CLI_SRCS ${CLI_DIR}/src/cli.c
             ${CLI_DIR}/src/commands.c)

idf_component_register(SRCS ${CLI_SRCS}
                       INCLUDE_DIRS ${CLI_DIR}/inc
                       PRIV_INCLUDE_DIRS ${CLI_DIR}/priv_inc
                       REQUIRES console nvs_flash)