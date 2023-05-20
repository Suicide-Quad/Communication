// Include Required Libraries

// Camera libraries
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"

//Wifi
#include <WiFi.h>

#include <string.h>
#include <math.h>


const char* ssid = "Redmi Note 9 Pro";
const char* password = "012345678";

const char* host = "192.168.43.96";
const int port = 2048;

WiFiClient client;


#define START_REQUEST 0xFE

#define SIZE_START 8

#define SIZE_TYPEFRAME 8

#define SIZE_CHECKSUM 8

#define SIZE_REQUEST(data) ((SIZE_START) + (SIZE_TYPEFRAME) + (data) + (SIZE_CHECKSUM))/8


/*___Struct and Enum___*/

int SizeTypeFrame [7] = {-1,8,24,136,40,72,131072};
enum TypeFrame
{
	NONE = -1,
	ASK_POSITION = 6,
	ACK = 1,
	RESPONSE_POSITION = 2,
	DEBUG_POSITION = 3,
	DEBUG_INT = 4,
	DEBUG_FLOAT = 5,
};

// Pin definitions for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19       
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22



int getSizeTypeFrame (enum TypeFrame type)
{
	if (type == NONE)
		return 0;
	return SizeTypeFrame[type];
}

// TODO : Merge with utils 
uint8_t computeCheckSum(int size, uint8_t* data)
{
	uint8_t sum = 0;
	for (int i = 0; i < size ; i ++)
	{
		sum += data[i];
	}
	return sum % 256;
}

void configESPCamera() 
{
  Serial.print("Initializing the camera module ");
  // Configure Camera parameters

  // Object to store the camera configuration parameters
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; // Choices are YUV422, GRAYSCALE, RGB565, JPEG

  // Select lower framesize if the camera doesn't support PSRAM
  if (psramFound()) 
  {
    config.frame_size = FRAMESIZE_SVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10; //10-63 lower number means higher quality
    config.fb_count = 2;
  } else 
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Initialize the Camera
  esp_err_t err = esp_camera_init(&config);
  while(err != ESP_OK)
  {
    delay(1000);
    Serial.print(".");
    err = esp_camera_init(&config);
  }

  // Camera quality adjustments
  sensor_t * s = esp_camera_sensor_get();

  // BRIGHTNESS (-2 to 2)
  s->set_brightness(s, 0);
  // CONTRAST (-2 to 2)
  s->set_contrast(s, 0);
  // SATURATION (-2 to 2)
  s->set_saturation(s, 0);
  // SPECIAL EFFECTS (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_special_effect(s, 0);
  // WHITE BALANCE (0 = Disable , 1 = Enable)
  s->set_whitebal(s, 1);
  // AWB GAIN (0 = Disable , 1 = Enable)
  s->set_awb_gain(s, 1);
  // WB MODES (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_wb_mode(s, 0);
  // EXPOSURE CONTROLS (0 = Disable , 1 = Enable)
  s->set_exposure_ctrl(s, 1);
  // AEC2 (0 = Disable , 1 = Enable)
  s->set_aec2(s, 0);
  // AE LEVELS (-2 to 2)
  s->set_ae_level(s, 0);
  // AEC VALUES (0 to 1200)
  s->set_aec_value(s, 300);                              //??
  // GAIN CONTROLS (0 = Disable , 1 = Enable)
  s->set_gain_ctrl(s, 1);
  // AGC GAIN (0 to 30)
  s->set_agc_gain(s, 0);                          //??         
  // GAIN CEILING (0 to 6)
  s->set_gainceiling(s, (gainceiling_t)0);               
  // BPC (0 = Disable , 1 = Enable)
  s->set_bpc(s, 0);
  // WPC (0 = Disable , 1 = Enable)
  s->set_wpc(s, 1);
  // RAW GMA (0 = Disable , 1 = Enable)
  s->set_raw_gma(s, 1);
  // LENC (0 = Disable , 1 = Enable)
  s->set_lenc(s, 1);
  // HORIZ MIRROR (0 = Disable , 1 = Enable)
  s->set_hmirror(s, 0);
  // VERT FLIP (0 = Disable , 1 = Enable)
  s->set_vflip(s, 0);
  // DCW (0 = Disable , 1 = Enable)
  s->set_dcw(s, 1);
  // COLOR BAR PATTERN (0 = Disable , 1 = Enable)
  s->set_colorbar(s, 0);

  Serial.println(" Camera OK");
}


void initWifi()
{
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" ");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Wifi OK");

  Serial.print("Conneting to server ");
  while(!client.connect(host, port))
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connection OK");
}

void setup() {

  // Disable brownout detector
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Start Serial Monitor
  Serial.begin(115200);

  //spi=SPIClass(VSPI);

  Serial.println("");
  Serial.println("____Setup____");
  // Initialize the camera wifi and connection to Server
  configESPCamera();
  initWifi();
  Serial.println("");
}


void takeNewPhoto(int send) 
{
  // Take Picture with Camera
  Serial.println("____Photo___");
  // Setup frame buffer
  camera_fb_t  * fb = esp_camera_fb_get();

  if (!fb) 
  {
    Serial.println("Camera capture failed");
    Serial.println("");
    return;
  }

  if (send)
  {
    uint8_t request[7];
    request[0] = START_REQUEST;
    request[1] = ASK_POSITION;
    Serial.print("Size send image : ");
    Serial.println(fb->len);
    memcpy(&request[2],&fb->len,4);
    int sum = computeCheckSum(4,&request[2]);
    request[6] = sum;
    client.write(request, 7);
    
    int nbSend = 0;
    int nbPackets = 0;
    while (nbSend < fb-> len)
    {
      int atSendNow = (fb->len-nbSend < atSendNow ? fb->len-nbSend : 1460);
      client.write(&fb->buf[nbSend],atSendNow);
      nbSend += atSendNow;
      nbPackets ++;
    }
    Serial.print("Image Send in ");
    Serial.print(nbPackets);
    Serial.println(" packets");
  }

  // Return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);
  if (!send)
    Serial.println("Photo taken");
  Serial.println("");
}

void sendRequest(uint8_t* buffer, size_t size)
{
  Serial.println("____Communication____");
  if(WiFi.status()== WL_CONNECTED)
  {
      Serial.println("Send to server : ");
      for (int i = 0; i < size; i++)
      {
        Serial.println(buffer[i], HEX);
      }
      
      client.write((char*)buffer, size);

      delay(100);
      
      Serial.println("Receive from server : ");
      while(client.available())
      {
        char read = client.read();
        Serial.println(read, HEX);
      } 
    }
    else 
    {
      Serial.println("WiFi Disconnected");
    }

    Serial.println("");
}

void loop() 
{
  while(Serial.available()<=0);
  
  enum TypeFrame type = NONE;
  
  int read = Serial.read();
  if (read == START_REQUEST)
  {
    type = (TypeFrame)Serial.read();
  }
  if (type != NONE)
  {
    if (type == ASK_POSITION)                      
    {
      delay(500);
      takeNewPhoto(0);
      takeNewPhoto(0);
      takeNewPhoto(0);
      takeNewPhoto(1);
    }
    else
    {
      uint8_t sizeType = getSizeTypeFrame(type);
      int sizeRequest = SIZE_REQUEST(sizeType);
      uint8_t request [sizeRequest];
      request[0] = START_REQUEST;
      request[1] = type;

      Serial.readBytes(&request[2], sizeType/8);

      uint8_t sum = computeCheckSum(sizeType/8, &request[2]);
      request[sizeRequest-1] = sum;
      sendRequest(request, sizeRequest);
    }
  }
}
