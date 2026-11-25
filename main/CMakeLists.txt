idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES spi_flash esp_app_format esp_timer
)

# Stricter warnings for project code only (ESP-IDF already enables -Wall).
target_compile_options(${COMPONENT_LIB} PRIVATE -Wextra -Wshadow)
