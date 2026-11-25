/**
 * @file main.c
 * @brief Level 1 / Step 1: boot banner + periodic heartbeat log.
 *
 * Verifies the toolchain, flashing over USB-Serial/JTAG and the log console.
 * The reset reason is printed at boot so that an unexpected reset loop can be
 * diagnosed from the log alone.
 */
#include <inttypes.h>
#include <stdint.h>

#include "esp_app_desc.h"
#include "esp_chip_info.h"
#include "esp_err.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_config.h"

static const char *TAG = "app";

static const char *reset_reason_to_str(esp_reset_reason_t reason)
{
    const char *str;

    switch (reason) {
    case ESP_RST_POWERON:
        str = "POWERON";
        break;
    case ESP_RST_EXT:
        str = "EXT";
        break;
    case ESP_RST_SW:
        str = "SW";
        break;
    case ESP_RST_PANIC:
        str = "PANIC";
        break;
    case ESP_RST_INT_WDT:
        str = "INT_WDT";
        break;
    case ESP_RST_TASK_WDT:
        str = "TASK_WDT";
        break;
    case ESP_RST_WDT:
        str = "WDT";
        break;
    case ESP_RST_DEEPSLEEP:
        str = "DEEPSLEEP";
        break;
    case ESP_RST_BROWNOUT:
        str = "BROWNOUT";
        break;
    case ESP_RST_USB:
        str = "USB";
        break;
    default:
        /* Any other value is reported numerically by the caller. */
        str = "OTHER";
        break;
    }

    return str;
}

static void log_boot_info(void)
{
    const esp_app_desc_t *app = esp_app_get_description();
    ESP_LOGI(TAG, "[app] boot: fw=%s idf=%s built=%s %s", app->version, app->idf_ver, app->date,
             app->time);

    esp_chip_info_t chip;
    esp_chip_info(&chip);
    ESP_LOGI(TAG, "[app] chip: model=%d rev=v%u.%u cores=%u", (int)chip.model,
             (unsigned)(chip.revision / 100U), (unsigned)(chip.revision % 100U),
             (unsigned)chip.cores);

    uint32_t flash_size = 0U;
    esp_err_t err = esp_flash_get_size(NULL, &flash_size);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "[app] flash: size=%" PRIu32 " bytes", flash_size);
    } else {
        /* Not fatal: the heartbeat still proves the system is running. */
        ESP_LOGW(TAG, "[app] flash: size read failed (err=0x%x)", (unsigned)err);
    }

    esp_reset_reason_t reason = esp_reset_reason();
    ESP_LOGI(TAG, "[app] reset: reason=%s (%d)", reset_reason_to_str(reason), (int)reason);
}

void app_main(void)
{
    log_boot_info();

    /* vTaskDelayUntil keeps a fixed period independent of log execution time. */
    const TickType_t period_ticks = pdMS_TO_TICKS(APP_HEARTBEAT_PERIOD_MS);
    TickType_t last_wake = xTaskGetTickCount();
    uint32_t count = 0U;

    for (;;) {
        const int64_t uptime_ms = esp_timer_get_time() / 1000;
        ESP_LOGI(TAG, "[app] alive: count=%" PRIu32 " uptime_ms=%" PRId64, count, uptime_ms);
        count++;
        vTaskDelayUntil(&last_wake, period_ticks);
    }
}
