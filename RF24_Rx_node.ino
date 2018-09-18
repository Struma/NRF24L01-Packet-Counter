/*
/ This code is working on an arduino nano v3 with another NRF24Radio programmed to submit the packet (code oncomeing)
/ This code relies on the RF24 Library
/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


unsigned long msg;
RF24 radio(10, 9); //the compact rf node is wired backwards


byte addresses[][6] = {"ball0", "1ball", "2ball", "3ball", "4ball", "5ball"};

int p_count = 0;
int package_count = 0;


int l_switch = 8;
int c_switch = 7;
int r_switch = 6;

byte sw_l = 0;
byte sw_c = 0;
byte sw_r = 0;





void setup(void) {
  radio.begin();
  radioConfig();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


  pinMode(l_switch, INPUT);
  pinMode(c_switch, INPUT);
  pinMode(r_switch, INPUT);


  //radio.openWritingPipe(addresses[3]);
  radio.openReadingPipe(0, addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.openReadingPipe(2, addresses[2]);
  radio.openReadingPipe(3, addresses[3]);

  radio.startListening();
  splash_screen(1000);
  node_display("NODE #1", "packages", p_count);
}


void loop(void) {
  
  refresh_buttons();
  if (sw_c == 1) {
    p_count = button_toggle(p_count, 2);
    delay(200);
  }
  
  if (radio.available()) { //Check if received data
    
    radio.read( &msg, sizeof(unsigned long)); //read one byte of data and store it in gotByte variable
    if (msg != 110) {
      package_count++;
      package_counter(package_count);
      
    }
  }  
}


void radioConfig() {
  radio.setRetries(15, 15);          //retry after 500us 15 times
  radio.setChannel(108);            // set channel to 108 (default is 76)
  //radio.enableDynamicPayloads();    //send messages at just the right size
  radio.setPALevel(RF24_PA_LOW);    // set transmitter power (MIN, LOW, HIGH, MAX)
  radio.setDataRate(RF24_250KBPS);  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps  return bool1 is success
  //radio.setAutoAck(true);         // is true by default
}

void splash_screen(int clk_s) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(F("Julio's   RF NODE   v1.0 "));
  display.display();
  delay(clk_s);
}

void refresh_buttons() {
  sw_l = digitalRead(l_switch);
  sw_c = digitalRead(c_switch);
  sw_r = digitalRead(r_switch);
}


// this function takes an input number and toggles it from 0 to X-"limit"
int button_toggle(int var, int limit) {
  int flag = 1;
  node_display("NODE Sel", "packages", var);
  delay(200);
  while (flag) {
    refresh_buttons();
    if (sw_l == 1) {
      if (var != 0) {
        var --;
      } else {
        ;
      }
      node_display("NODE Sel", "packages", var);
      delay(150);
    } else if (sw_r == 1) {
      if (var < limit) {
        var ++;
      } else {
        var = limit;
      }
      node_display("NODE Sel", "packages", var);
      delay(150);
    } else if (sw_c == 1) {
      flag = 0;
      node_display("NODE #1", "packages", var);
      delay(150);
    }
  }
  return var;
}

void node_display(String title, String message, int cursr) {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(title);

  display.setTextSize(1);
  display.setCursor(0, 18);
  display.print(message);

  if (cursr == 0) {
    display.setCursor(100, 18);
    display.print("*");
  }

  if (cursr == 1) {
    display.setCursor(100, 33);
    display.print("*");
  }

  if (cursr == 2) {
    display.setCursor(100, 48);
    display.print("*");
  }

  display.display();
}


void package_counter(int var) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("#Pkg found");
  display.setTextSize(4);
  display.setCursor(0, 18);
  display.print(var);
  display.display();
}



