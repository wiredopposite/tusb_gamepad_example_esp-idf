idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES boards tinyusb_src tusb_gamepad esp_timer)
