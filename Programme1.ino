#include <EEPROM.h>
#include <esp_camera.h>
#include <png_encoder.h>

#define EEPROM_SIZE 4096  // Size of the EEPROM in bytes
#define EEPROM_ADDR 0     // Starting address of the image in the EEPROM

void setup() {
  Serial.begin(115200);

  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = -1;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x", err);
    return;
  }

  // Capture an image
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Encode the image as a PNG file
  uint8_t *png_data;
  size_t png_size;
  encode_image_as_png(fb->buf, fb->width, fb->height, &png_data, &png_size);

  // Store the PNG file in the EEPROM
  if (png_size > EEPROM_SIZE) {
    Serial.println("PNG file too large to fit in EEPROM");
    return;
  }
  for (size_t i = 0; i < png_size; i++) {
    EEPROM.write(EEPROM_ADDR + i, png_data[i]);
  }

  Serial.printf("Image captured and stored in EEPROM at address %d, size %d bytes", EEPROM_ADDR, png_size);

  // Clean up
  esp_camera_fb_return(fb);
  free(png_data);
}

void loop() {
  // Do nothing
}
//
