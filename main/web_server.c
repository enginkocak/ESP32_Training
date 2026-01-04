#include "web_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "led.h"
#include <string.h>

static const char *TAG = "web";

static esp_err_t root_get_handler(httpd_req_t *req)
{
    const char *html =
        "<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>ESP32 LED</title></head><body>"
        "<h2>ESP32 SoftAP Web LED</h2>"
        "<p>LED State: <span id='st'>?</span></p>"
        "<button onclick=\"fetch('/led/on',{method:'POST'}).then(update)\">LED ON</button> "
        "<button onclick=\"fetch('/led/off',{method:'POST'}).then(update)\">LED OFF</button>"
        "<script>"
        "function update(){fetch('/led/state').then(r=>r.text()).then(t=>document.getElementById('st').innerText=t);}"
        "update();"
        "</script>"
        "</body></html>";

    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t led_on_post_handler(httpd_req_t *req)
{
    led_set(true);
    return httpd_resp_sendstr(req, "OK");
}

static esp_err_t led_off_post_handler(httpd_req_t *req)
{
    led_set(false);
    return httpd_resp_sendstr(req, "OK");
}

static esp_err_t led_state_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/plain");
    return httpd_resp_sendstr(req, led_get() ? "ON" : "OFF");
}

void web_server_start(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    httpd_handle_t server = NULL;
    esp_err_t err = httpd_start(&server, &config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "httpd_start failed: %d", err);
        return;
    }

    httpd_uri_t uri_root = {
        .uri      = "/",
        .method   = HTTP_GET,
        .handler  = root_get_handler
    };
    httpd_uri_t uri_led_on = {
        .uri      = "/led/on",
        .method   = HTTP_POST,
        .handler  = led_on_post_handler
    };
    httpd_uri_t uri_led_off = {
        .uri      = "/led/off",
        .method   = HTTP_POST,
        .handler  = led_off_post_handler
    };
    httpd_uri_t uri_led_state = {
        .uri      = "/led/state",
        .method   = HTTP_GET,
        .handler  = led_state_get_handler
    };

    httpd_register_uri_handler(server, &uri_root);
    httpd_register_uri_handler(server, &uri_led_on);
    httpd_register_uri_handler(server, &uri_led_off);
    httpd_register_uri_handler(server, &uri_led_state);

    //ESP_LOGI(TAG, "Web server started.");
}
