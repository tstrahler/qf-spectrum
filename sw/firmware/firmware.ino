#include <SPI.h>
#include <hardware/dma.h>

// Clock input to sensor
#define GPIO_CLK  21
// Start signal to sensor
#define GPIO_ST   5

// End-of-conversion - used as CS signal, provided by sensor
#define GPIO_CS   17
// D.Trig - used as serial CLK signal, provided by sensor
#define GPIO_SCK  18
// Digial-Output - used as serial data in signal, provided by sensor
#define GPIO_SDI  16

// Debug GPIO
#define GPIO_DBG  7

#define PIX_COUNT 1024

int int_time = 100;

void setup() {
  // Init serial port
  Serial.begin(115200);  

  // Output 5MHz CLK to GPIO21
  clock_gpio_init(GPIO_CLK, CLOCKS_CLK_REF_CTRL_SRC_VALUE_ROSC_CLKSRC_PH, 1250);

  // Set up GPIO5 to output ST signal
  gpio_init(GPIO_ST);
  gpio_set_dir(GPIO_ST, GPIO_OUT);
  // Set 7p GPIO7 for debugging
  gpio_init(GPIO_DBG);
  gpio_set_dir(GPIO_DBG, GPIO_OUT);
  
  // Set up SPI as slave
  spi_init(spi0, 10000);
  spi_set_slave(spi0, true);
  spi_set_format(spi0, 10, SPI_CPOL_1, SPI_CPHA_0, SPI_MSB_FIRST);
  // Set up GPIO of SPI
  gpio_set_function(GPIO_CS, GPIO_FUNC_SPI);
  gpio_set_function(GPIO_SCK, GPIO_FUNC_SPI);
  gpio_set_function(GPIO_SDI, GPIO_FUNC_SPI);

}

void loop() {
  // Set ST pin low for 5us to initialize transfer
  gpio_clr_mask(1 << GPIO_ST);

  // Poll data from sensor
  uint16_t rx_data[PIX_COUNT];
  for(int i = 0; i < PIX_COUNT; i++){
    spi_read16_blocking(spi0, 0x00, rx_data + i, 1);
  }
  gpio_set_mask(1 << GPIO_ST);

  // Transmit data via serial
  Serial.println(0xFFFF);
  for(int i = 0; i < PIX_COUNT; i++) Serial.println(rx_data[i]);
  
  delay(100);
  
  
  

    
} 
