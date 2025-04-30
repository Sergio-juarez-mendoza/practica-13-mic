cmake_minimum_required(VERSION 3.13)

# Nombre del proyecto
project(Practica_13)

# Habilitar la Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)
pico_sdk_init()

# Agregar el archivo fuente
add_executable(LEDRGB
    LEDRGB.c
)

# Agregar las bibliotecas estándar de la Pico
target_link_libraries(LEDRGB
    pico_stdlib
    hardware_adc
    hardware_pwm
)

# Habilitar la salida USB para el puerto serie
pico_enable_stdio_usb(LEDRGB 1)
pico_enable_stdio_uart(LEDRGB 0)

# Crear el archivo binario UF2
pico_add_extra_outputs(LEDRGB)
