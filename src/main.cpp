#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Esp.h>

static TFT_eSPI tft;

static const uint32_t IMAGE_HEIGHT = 128;
static const uint32_t IMAGE_WIDTH = 180;

static uint16_t sprite_image[IMAGE_HEIGHT * IMAGE_WIDTH];
static Print& logger = Serial;

static void prepare();
static void tick();

void setup(void)
{
  Serial.begin(115200);
  Serial.println("");

  logger.println("start");
  tft.init();
  prepare();
  tft.fillScreen(ILI9163_GREENYELLOW);
}

void loop(void)
{
  for(int i=0;i<50;i++)
  {
    tick();
  }
  yield();
  //logger.println("loop");
  //delay(100);
}

static void memset16(uint16_t* ptr, uint16_t val, size_t size)
{
    while (size--)
    {
        *ptr = val;
        ptr++;
    }
}

static const uint16_t colorCycle[] =  {ILI9163_MAGENTA, ILI9163_YELLOW, ILI9163_DARKGREEN, ILI9163_DARKCYAN, ILI9163_MAROON, ILI9163_PURPLE, ILI9163_OLIVE, ILI9163_LIGHTGREY, 
                               ILI9163_RED, ILI9163_NAVY, ILI9163_DARKGREY, ILI9163_BLUE, ILI9163_GREEN, ILI9163_CYAN, ILI9163_MAGENTA, ILI9163_YELLOW, 
                               ILI9163_ORANGE, ILI9163_GREENYELLOW};

static void prepare()
{
    memset16(sprite_image, ILI9163_YELLOW, IMAGE_HEIGHT*IMAGE_WIDTH);
    uint16_t* ptr = sprite_image;
    for(uint32_t y=0; y < IMAGE_HEIGHT; y++)
    {
        uint32_t x = 0;
        uint16_t colorIndex = 0;
        while (x < IMAGE_WIDTH)
        {
          memset16(ptr, colorCycle[colorIndex], 10);
          x += 10;
          ptr += 10;
          colorIndex++;
          colorIndex %= 18;
        }
    }
}


static uint32_t pos = 0;
static bool dir = true;
static uint16_t counter = 0;
static void tick()
{
  uint16_t* ptr = sprite_image + pos;
  tft.cs_l();
  tft.setAddrWindow(0, 0, 127, IMAGE_HEIGHT - 1);
  for(uint32_t y=0; y < IMAGE_HEIGHT; y++)
  {
      SPI.writeBytes((uint8_t*)ptr, 128*sizeof(uint16_t));
      ptr+=IMAGE_WIDTH;
  }
  tft.cs_h();

  if (dir)
    pos+=2;
  else
    pos-=2;

  if ((pos >= (IMAGE_WIDTH - 128)) || (pos == 0))
  {
    dir = !dir;
    counter++;
    logger.println(ESP.getFreeHeap());
  }
}