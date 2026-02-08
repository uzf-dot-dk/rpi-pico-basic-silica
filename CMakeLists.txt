cmake_minimum_required(VERSION 3.28)

include(pico_sdk_import.cmake)

project( blinker C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_compile_options(-Wno-maybe-uninitialized)

set(FAMILY rp2040)
set(BOARDpico_sdk)

add_executable(
    ${PROJECT_NAME}
    main.c
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} main 0)


target_link_libraries(${PROJECT_NAME} PRIVATE pico_stdlib hardware_resets hardware_irq pico_unique_id)
pico_add_extra_outputs(${PROJECT_NAME})

add_custom_target( flash
    COMMAND killall -9 minicom || true
    COMMAND picotool reboot -uf
    COMMAND sleep 1
    COMMAND picotool load -f "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.uf2"
    COMMAND picotool reboot
    DEPENDS ${PROJECT_NAME}
    COMMENT "Flashing ${PROJECT_NAME}.uf2 to Pico using picotool"
)
