#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#include "tusb.h"
#include "bsp/board_api.h"
#include "tusb_gamepad.h"

#define PI 3.14159265358979323846
#define MAX_ANGLE 360

void update_gamepad(Gamepad* gp)
{
    // Increment triggers
    if (gp->triggers.l < UINT8_MAX) 
    {
        gp->triggers.l++;
        gp->triggers.r++;
    } 
    else 
    {
        gp->reset_triggers(gp); // Reset triggers to zero
    }

    // Rotate joysticks
    static int angle = 0;
    float radian = angle * (PI / 180.0);

    gp->joysticks.lx = (int16_t)(cos(radian) * INT16_MAX);
    gp->joysticks.ly = (int16_t)(sin(radian) * INT16_MAX);
    gp->joysticks.rx = (int16_t)(sin(radian) * INT16_MAX);
    gp->joysticks.ry = (int16_t)(cos(radian) * INT16_MAX);

    angle = (angle + 5) % MAX_ANGLE;

    // Change button press every 250ms
    static unsigned long last_time = 0;
    unsigned long current_time = esp_timer_get_time() / 1000;
    static int current_button = 0;

    if (current_time - last_time >= 250) 
    {
        last_time = current_time;

        gp->reset_buttons(gp); // Reset all buttons

        switch (current_button)
        {
            case 0:
                gp->buttons.up = true;
                current_button++;
                break;
            case 1:
                gp->buttons.down = true;
                current_button++;
                break;
            case 2:
                gp->buttons.left = true;
                current_button++;
                break;
            case 3:
                gp->buttons.right = true;
                current_button++;
                break;
            case 4:
                gp->buttons.a = true;
                current_button++;
                break;
            case 5:
                gp->buttons.b = true;
                current_button++;
                break;
            case 6:
                gp->buttons.x = true;
                current_button++;
                break;
            case 7:
                gp->buttons.y = true;
                current_button++;
                break;
            case 8:
                gp->buttons.l3 = true;
                current_button++;
                break;
            case 9:
                gp->buttons.r3 = true;
                current_button++;
                break;
            case 10:
                gp->buttons.back = true;
                current_button++;
                break;
            case 11:
                gp->buttons.start = true;
                current_button++;
                break;
            case 12:
                gp->buttons.rb = true;
                current_button++;
                break;
            case 13:
                gp->buttons.lb = true;
                current_button++;
                break;
            case 14:
                gp->buttons.sys = true;
                current_button++;
                break;
            case 15:
                gp->buttons.misc = true;
                current_button = 0;
                break;
        }
    }
}

void update_gp_xtask(void* param)
{
    (void) param;

    Gamepad* gp = gamepad(0); // Get gamepad object, pass 0 as arg if only using 1 gamepad

    while (1)
    {
        update_gamepad(gp);
        vTaskDelay(1);
    }
}

void tusb_gamepad_xtask(void* param)
{
    (void) param;

    board_init();

    enum InputMode input_mode = INPUT_MODE_XINPUT; // Set an input mode
    
    init_tusb_gamepad(input_mode); // Initializes USB driver

    while (1)
    {
        tusb_gamepad_task(); // Send/receive gamepad data over USB
        vTaskDelay(1);
    }
}

void usb_xtask(void* param)
{
    (void) param;

    if (board_init_after_tusb) 
    {
        board_init_after_tusb();
    }

    while (1)
    {
        tud_task(); // Put this thread to waiting state until there is new events, doesn't return
    }
}

void app_main(void)
{
    xTaskCreate(tusb_gamepad_xtask, "tusb_gamepad_xtask", 4096, NULL, configMAX_PRIORITIES-2, NULL);
    xTaskCreate(usb_xtask, "usb_xtask", 4096, NULL, configMAX_PRIORITIES-1, NULL); 
    xTaskCreate(update_gp_xtask, "gp_xtask", 4096, NULL, configMAX_PRIORITIES-3, NULL);
}