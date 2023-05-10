
add_library(freemodbus-rp2040 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/port/portevent.c
    ${CMAKE_CURRENT_LIST_DIR}/port/portserial.c
    ${CMAKE_CURRENT_LIST_DIR}/port/porttimer.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/ascii/mbascii.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbfunccoils.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbfuncdiag.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbfuncdisc.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbfuncholding.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbfuncinput.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbfuncother.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/functions/mbutils.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/mb.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/rtu/mbcrc.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/rtu/mbrtu.c
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/tcp/mbtcp.c
)

target_link_libraries(freemodbus-rp2040
    pico_time
    hardware_timer
    hardware_uart
    hardware_irq
)

target_include_directories(freemodbus-rp2040 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port/
    ${CMAKE_CURRENT_LIST_DIR}/./
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/rtu/
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/ascii/
    ${CMAKE_CURRENT_LIST_DIR}/../../modbus/include/
)
