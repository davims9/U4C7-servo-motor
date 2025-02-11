#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define LED_PIN 12
#define WRAP_VALUE 20000 // Período de 20ms em ciclos de clock

void set_servo_angle(uint slice, uint channel, uint32_t pulse_width_us) {
    uint32_t level = (pulse_width_us * WRAP_VALUE) / 20000; // Converte para nível de PWM
    pwm_set_chan_level(slice, channel, level);
}

int main() {
    stdio_init_all();
    
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // Ajusta o divisor de clock para atingir 50Hz
    pwm_config_set_wrap(&config, WRAP_VALUE);
    
    pwm_init(slice_num, &config, true);


    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num_led = pwm_gpio_to_slice_num(LED_PIN);
    uint channel_led = pwm_gpio_to_channel(LED_PIN);
  
    pwm_init(slice_num_led, &config, true);
    
    // Posição 180 graus (2400us)
    printf("Movimento para 180º\n");
    set_servo_angle(slice_num, channel, 2400);
    set_servo_angle(slice_num_led, channel_led, 2400);
    sleep_ms(5000);
    
    // Posição 90 graus (1470us)
    printf("Movimento para 90º\n");
    set_servo_angle(slice_num, channel, 1470);
    set_servo_angle(slice_num_led, channel_led, 1470);
    sleep_ms(5000);
    
    // Posição 0 graus (500us)
    printf("Movimento para 0º\n");
    set_servo_angle(slice_num, channel, 500);
    set_servo_angle(slice_num_led, channel_led, 500);
    sleep_ms(5000);
    
    printf("Movimentação suave +\n");
    while(true){
        // Movimento suave de 0 a 180 graus
        for (uint32_t pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_angle(slice_num, channel, pulse);
            set_servo_angle(slice_num_led, channel_led, pulse);
            sleep_ms(10);
        }
        
        // Movimento suave de 180 a 0 graus
        for (uint32_t pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_angle(slice_num, channel, pulse);
            set_servo_angle(slice_num_led, channel_led, pulse);
            sleep_ms(10);
        }
    }
    
}
