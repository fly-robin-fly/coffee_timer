# main/CMakeLists.txt
idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES ui lvgl spi_flash)