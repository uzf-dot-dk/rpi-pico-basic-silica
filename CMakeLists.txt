cmake_minimum_required(VERSION 3.28)

include(pico_sdk_import.cmake)

project( blinker C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_compile_options(-Wno-maybe-uninitialized)

set(FAMILY rp2040)
set(BOARD pico_sdk)

set(SILICA_BUILD_SANDBOX OFF CACHE BOOL "Disable silica sandbox")
set(SILICA_BUILD_TESTS   OFF CACHE BOOL "Disable silica unittests")

add_subdirectory( silica )

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} main 0)


target_link_libraries(${PROJECT_NAME} PRIVATE 
			pico_stdlib 
			hardware_resets 
			hardware_irq 
			pico_unique_id

			silica )

pico_add_extra_outputs(${PROJECT_NAME})

add_custom_target( flash
    COMMAND pkill pico-minicom || true
    COMMAND picotool reboot -uf
    COMMAND sleep 1
    COMMAND picotool load -f "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.uf2"
    COMMAND picotool reboot
    DEPENDS ${PROJECT_NAME}
    COMMENT "Flashing ${PROJECT_NAME}.uf2 to Pico using picotool"
)
