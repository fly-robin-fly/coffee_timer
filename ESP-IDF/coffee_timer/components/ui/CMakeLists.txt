# Find all source files recursively
file(GLOB_RECURSE SOURCES "*.c")

# Register the UI directory as an ESP-IDF component
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES lvgl)