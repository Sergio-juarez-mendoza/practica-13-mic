#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define PIN_ROJO 16   // GPIO para LED Rojo
#define PIN_VERDE 18  // ⚠️ Se cambió el pin del Verde
#define PIN_AZUL 17   // ⚠️ Se cambió el pin del Azul

void init_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, 4095);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(pin), 0);
    pwm_set_enabled(slice, true);
}

int main() {
    stdio_init_all();
    adc_init();

    // Configurar ADC en pines GP26, GP27, GP28
    adc_gpio_init(26);  // Potenciómetro Rojo
    adc_gpio_init(27);  // Potenciómetro Azul (se intercambió con el verde)
    adc_gpio_init(28);  // Potenciómetro Verde (se intercambió con el azul)

    // Configurar PWM en pines del LED RGB
    init_pwm(PIN_ROJO);
    init_pwm(PIN_VERDE);
    init_pwm(PIN_AZUL);

    while (1) {
        // Leer valores del ADC en el orden correcto
        adc_select_input(0);
        uint16_t valor_rojo = adc_read();
        adc_select_input(2);  // ⚠️ Se intercambiaron Verde y Azul
        uint16_t valor_verde = adc_read();
        adc_select_input(1);
        uint16_t valor_azul = adc_read();

        // Convertir a porcentaje
        float porcentaje_rojo = (valor_rojo / 4095.0) * 100.0;
        float porcentaje_verde = (valor_verde / 4095.0) * 100.0;
        float porcentaje_azul = (valor_azul / 4095.0) * 100.0;

        // Mostrar valores en el monitor serial
        printf("Rojo: %.2f%%  Verde: %.2f%%  Azul: %.2f%%\n", porcentaje_rojo, porcentaje_verde, porcentaje_azul);

        // Invertir PWM solo para LED de ánodo común
        uint16_t pwm_rojo = 4095 - valor_rojo;
        uint16_t pwm_verde = 4095 - valor_verde;
        uint16_t pwm_azul = 4095 - valor_azul;

        // Aplicar PWM a los LEDs
        pwm_set_gpio_level(PIN_ROJO, pwm_rojo);
        pwm_set_gpio_level(PIN_VERDE, pwm_verde);
        pwm_set_gpio_level(PIN_AZUL, pwm_azul);

        // Espera antes de la siguiente lectura
        sleep_ms(100);
    }
}
