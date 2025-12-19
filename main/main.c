#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "wifi_soft_AP.h"
#include "web_server.h"
#include "led.h"


void app_main(void)
{
    // NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    // TCP/IP + event loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // LED
    led_init();

    // Wi-Fi SoftAP start
    wifi_soft_AP_start();
    
    // Web server
    web_server_start();
}

