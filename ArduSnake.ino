#include <Arduboy2.h>

#define BLOCK_WIDTH 4
#define MAX_X (WIDTH / BLOCK_WIDTH)
#define MAX_Y (HEIGHT / BLOCK_WIDTH)
#define MAX_SNAKE 120
#define INITIAL_LENGTH 3
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3
#define MAX_PLAYERS 3
#define MAX_SPEED 3
#define BEEP_FREQ 1046.5
#define DEAD_FREQ 739.99

struct Block {
  byte x;
  byte y;
};

struct StartConditions {
  byte x;
  byte y;
  byte direction;
};

struct Snake {
  struct Block *chunks;
  unsigned short length;
  byte direction;
  bool dead;
  bool control_permitted;
};

const unsigned char TITLE_SCREEN_LEFT[] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0x70, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0x60, 0x60, 0x60, 0xe0, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0xf8, 0xfc, 0x7c, 0x3e, 0x3e, 0x1e, 0x1f, 0xf, 0xf, 0xf, 0xf, 0xf, 0x8f, 0x87, 0xe7, 0xff, 0xff, 0xff, 0x1f, 0x3, 0x3, 0x1, 0x7d, 0xfe, 0xff, 0xff, 0xbb, 0xd9, 0xd9, 0x99, 0xf9, 0xf9, 0xfb, 0xfb, 0xff, 0x7f, 0x7f, 0x3f, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xff, 0xff, 0x7f, 0x7f, 0x63, 0x63, 0x63, 0x43, 0x43, 0x83, 0x83, 0x7, 0x7, 0x7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x6, 0x2, 0x1, 0x81, 0x40, 0x00, 0x00, 0x18, 0xc, 0xc, 0xf, 0x3c, 0x38, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xec, 0xe7, 0xed, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0x3f, 0xf, 0xf, 0x1e, 0x18, 0x38, 0x38, 0x30, 0x00, 0x00, 0x00, 0x81, 0xe3, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x3, 0x7, 0x7, 0xf, 0x3c, 0x78, 0xf8, 0xf8, 0xf8, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0x18, 0x00, 0x3, 0x3, 0x7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x7, 0x7, 0x7, 0x7, 0xf, 0x1f, 0x1f, 0x1c, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xf1, 0xf0, 0x78, 0x3e, 0x3f, 0x1f, 0xf, 0x3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x7, 0xf, 0xf, 0x1e, 0x1e, 0x3e, 0x3f, 0x7c, 0x7c, 0xf8, 0xf0, 0xf8, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x80, 0x80, 0xc0, 0xe0, 0x7c, 0x7c, 0x3f, 0x3f, 0x1f, 0x1f, 0x9f, 0x8f, 0x8f, 0x87, 0x3, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 0x70, 0xf0, 0xf8, 0xf8, 0xfc, 0xfc, 0xfc, 0x3e, 0x1e, 0xe, 0x6, 0x7, 0x7, 0xc3, 0xe3, 0xf1, 0xf0, 0x18, 0x18, 0x3c, 0x1c, 0xe, 0xe, 0xcc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf9, 0x39, 0x19, 0x1f, 0xff, 0xff, 0xff, 0xf3, 0xf3, 0xf1, 0xf1, 0xf1, 0xf1, 0x18, 0x18, 0x7c, 0x7c, 0x3e, 0x3f, 0x3f, 0x1f, 0x1f, 0x13, 0x3, 0xc3, 0xc7, 0x47, 0x47, 0x46, 0x46, 0x4e, 0x7c, 0x38, 0x30, 0x00, 0x00, 0xe0, 0xf1, 0xf9, 0x7f, 0x3f, 0x3f, 0xff, 0xf8, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc1, 0xc3, 0xc3, 0x82, 0x82, 0x86, 0x84, 0x84, 0x84, 0x84, 0x84, 0x88, 0x88, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0xcc, 0xcc, 0xc4, 0xc4, 0xc4, 0xe4, 0xe6, 0xe2, 0xf2, 0xf3, 0xf1, 0xf1, 0xf9, 0xf8, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x10, 0x30, 0x30, 0x21, 0x60, 0x40, 0x40, 0xc7, 0x87, 0x8f, 0x83, 0x81, 0x11, 0x1f, 0x1f, 0x3f, 0x3f, 0x7, 0x7, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x3, 0x3, 0x7, 0x7, 0xf, 0xf, 0x87, 0x87, 0xcf, 0x47, 0x67, 0x23, 0x21, 0x11};
const unsigned char TITLE_SCREEN_RIGHT[] PROGMEM = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x88, 0x88, 0x88, 0x88, 0x8, 0x00, 0xf8, 0xf8, 0x38, 0xf0, 0xc0, 0x80, 0xf0, 0xf0, 0x00, 0xf8, 0xf8, 0x18, 0x18, 0x18, 0x18, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xc0, 0xe0, 0x70, 0x38, 0x00, 0xf8, 0xf8, 0x98, 0x98, 0x98, 0x18, 0x00, 0x00, 0x00, 0xf, 0xe, 0xfe, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x1b, 0x1b, 0x1b, 0x1b, 0x19, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x3, 0x7, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x3, 0x3, 0x3, 0x3, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x1, 0x3, 0x1f, 0x1e, 0x00, 0x1f, 0x1f, 0x19, 0x19, 0x18, 0x18, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x7f, 0x1f, 0x3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x9e, 0x1e, 0x1e, 0x3e, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfc, 0xfc, 0xfc, 0xfc, 0xc, 0x8, 0x10, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x19, 0x19, 0x8, 0x8, 0x8, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0x4, 0x4, 0x4, 0xc, 0x8, 0x8, 0x8, 0x8, 0x11, 0x31, 0x60, 0xc0, 0x87, 0x8e, 0x98, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x22, 0x22, 0x00, 0x3e, 0x8, 0x3e, 0x00, 0x3e, 0x22, 0x3e, 0x00, 0x3e, 0x22, 0x3e, 0x00, 0x2, 0x3e, 0x2, 0x00, 0x22, 0x3a, 0x22, 0x00, 0x3e, 0x2, 0x3c, 0x00, 0x00};
const unsigned char PAUSE_GRAPHIC[] PROGMEM = {0xff, 0x3, 0x1, 0x1, 0xe1, 0xa1, 0xe1, 0x1, 0xe1, 0xa1, 0xe1, 0x1, 0xe1, 0x1, 0xe1, 0x1, 0xe1, 0xa1, 0xa1, 0x1, 0xe1, 0xa1, 0xa1, 0x1, 0xe1, 0x21, 0x21, 0xc1, 0x1, 0x1, 0x3, 0xff, 0xff, 0xc0, 0x80, 0x80, 0x87, 0x80, 0x80, 0x80, 0x87, 0x80, 0x87, 0x80, 0x87, 0x84, 0x87, 0x80, 0x84, 0x84, 0x87, 0x80, 0x87, 0x84, 0x84, 0x80, 0x87, 0x84, 0x84, 0x83, 0x80, 0x80, 0xc0, 0xff};
const unsigned char DEATH_BOX[] PROGMEM = {0xfe, 0x1, 0xf1, 0x5d, 0xcd, 0xbd, 0xcd, 0x5d, 0xf1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0xfe, 0xff, 0x00, 0x8, 0xb, 0xa, 0xa, 0xa, 0xb, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x00, 0xff, 0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3f};

const StartConditions player_start_conditions[3] = {{1, 1, RIGHT}, {MAX_X - 1, MAX_Y / 2, LEFT}, {1, MAX_Y - 1, RIGHT}};

Arduboy2 arduboy;
BeepPin1 beep1;
struct Snake *snakes;
struct Block food;
bool paused = false;
byte number_of_players = 1;
byte game_speed = 3;
uint8_t tone_length = 5;

void setup() {
  arduboy.begin();
  arduboy.initRandomSeed();
  
  beep1.begin();

  uint8_t framerate = 20;
  
  // FrameRate for the Title Screen.
  arduboy.setFrameRate(framerate);
  set_tone_length(framerate);

  // Separate loop for just the title screen. Will require additional setup after this has taken place.
  title_screen();
  
  switch (game_speed) {
    case 1:
      framerate = 10;
      break;
    case 2:
      framerate = 15;
      break;
    case 3:
      framerate = 20;
      break;
  }
  arduboy.setFrameRate(framerate);
  set_tone_length(framerate);
  
  snakes = (struct Snake *) malloc(sizeof(struct Snake) * number_of_players);
  for (int i = 0; i < number_of_players; i++) {
    struct Block *chunks = (struct Block *) malloc(sizeof(struct Block) * MAX_SNAKE);
    struct Snake snake = {chunks, INITIAL_LENGTH, RIGHT, false, true};
    snakes[i] = snake;
  }
  reset();
}

void set_tone_length(uint8_t framerate) {
  tone_length = framerate * 0.05;

  if (tone_length == 0) {
    tone_length = 1;
  }
}

void title_screen() {
  int current_option = 0;
  arduboy.clear();
  char *title_screen = TITLE_SCREEN_LEFT;
  arduboy.drawBitmap(0, 0, title_screen, 64, 64, WHITE);
  title_screen = TITLE_SCREEN_RIGHT;
  arduboy.drawBitmap(64, 0, title_screen, 64, 64, WHITE);

  bool play_selected = false;
  while (true) {
    if (!arduboy.nextFrame()) {
      continue;
    }

    Arduboy2Base::pollButtons();
    
    beep1.timer();

    bool button_pressed = false;
    
    if (Arduboy2Base::justPressed(B_BUTTON)) {
      switch (current_option) {
        case 0:
          play_selected = true;
          break;
        case 1:
          number_of_players++;
          if (number_of_players > MAX_PLAYERS) {
            number_of_players = 1;
          }
          break;
        case 2:
          game_speed++;
          if (game_speed > MAX_SPEED) {
            game_speed = 1;
          }
          break;
      }
      button_pressed = true;
    }

    if (Arduboy2Base::justPressed(DOWN_BUTTON)) {
      current_option = (current_option + 1) % 3;
      button_pressed = true;
      beep1.tone(beep1.freq(BEEP_FREQ), tone_length);
    }

    if (Arduboy2Base::justPressed(UP_BUTTON)) {
      current_option--;
      if (current_option < 0) {
        current_option = 2;
      }
      button_pressed = true;
    }

    if (button_pressed) {
      beep1.tone(beep1.freq(BEEP_FREQ), tone_length);
    }

    if (play_selected) {
      break;
    }

    arduboy.setCursor(79, 18);
    if (current_option == 0) {
      arduboy.print("> ");
    } else {
      arduboy.print("  ");
    }
    arduboy.print("Play!");
    arduboy.setCursor(79, 27);
    if (current_option == 1) {
      arduboy.print("> ");
    } else {
      arduboy.print("  ");
    }
    arduboy.print("Ps:");
    arduboy.setCursor(120, 27);
    arduboy.print(number_of_players);
    arduboy.setCursor(79, 36);
    if (current_option == 2) {
      arduboy.print("> ");
    } else {
      arduboy.print("  ");
    }
    arduboy.print("Spd:");
    arduboy.setCursor(120, 36);
    arduboy.print(game_speed);
    arduboy.display();
  }
}

void loop() {
  process_input();
  if (!arduboy.nextFrame()) {
    return;
  }

  beep1.timer();

  if (!paused && !all_players_dead()) {
    for (int i = 0; i < number_of_players; i++) {
      move_snake(i);
    }

    for (int i = 0; i < number_of_players; i++) {
      if (snake_collided(i)) {
        beep1.tone(beep1.freq(DEAD_FREQ), tone_length * 2);
        if (number_of_players == 1) {
          snakes[i].dead = true; 
        } else {
          spawn_snake(i);
        }
      } else if (food_collided(i)) {
        eat_food(i);
      }
    }
  }
  
  arduboy.clear();
  draw_food();
  draw_snakes();
  draw_interface();
  arduboy.display();
}

bool all_players_dead() {
  for (int i = 0; i < number_of_players; i++) {
    if (!snakes[i].dead) {
      return false;
    }
  }
  return true;
}

void process_input() {
  if (number_of_players == 1) {
    single_player_controls();
  } else {
    multiplayer_controls();
  }
}

void multiplayer_controls() {
  Arduboy2Base::pollButtons();
  //Snake 1
  if (Arduboy2Base::justPressed(LEFT_BUTTON)) {
    if (!snakes[0].dead) {
      rotate_snake(0, false);
    }
  }

  if (Arduboy2Base::justPressed(RIGHT_BUTTON)) {
    if (all_players_dead()) {
      reset();
    } else if (!snakes[0].dead) {
      rotate_snake(0, true);
    }
  }

  if (paused || all_players_dead()) {
    return;
  }

  //Snake 2
  if (Arduboy2Base::justPressed(A_BUTTON)) {
    if (!snakes[1].dead) {
      rotate_snake(1, false);
    }
  }

  if (Arduboy2Base::justPressed(B_BUTTON)) {
    if (!snakes[1].dead) {
      rotate_snake(1, true);
    }
  }

  //Snake 3
  if (Arduboy2Base::justPressed(UP_BUTTON)) {
    if (!snakes[2].dead) {
      rotate_snake(2, false);
    }
  }

  if (Arduboy2Base::justPressed(DOWN_BUTTON)) {
    if (!snakes[2].dead) {
      rotate_snake(2, true);
    }
  }
}

void rotate_snake(int player, bool clockwise) {
  if (snakes[player].control_permitted) {
    if (clockwise) {
      snakes[player].direction++;
      if (snakes[player].direction == 4) {
        snakes[player].direction = 0;
      }
      snakes[player].control_permitted = false;
    } else {
      snakes[player].direction--;
      if (snakes[player].direction == 255) {
        snakes[player].direction = 3;
      }
      snakes[player].control_permitted = false;
    }
  }
}

void single_player_controls() {
  Arduboy2Base::pollButtons();
  /*
  if (Arduboy2Base::justPressed(A_BUTTON)) {
  }
  */

  if (Arduboy2Base::justPressed(B_BUTTON)) {
    if (snakes[0].dead) {
      reset();
    } else {
      paused = !paused;
    }
  }

  if (paused || snakes[0].dead) {
    return;
  }

  if (snakes[0].control_permitted) {
    if (Arduboy2Base::justPressed(LEFT_BUTTON)) {
      if (!(paused || snakes[0].dead)) {
        if (snakes[0].direction != RIGHT) {
          snakes[0].direction = LEFT;
          snakes[0].control_permitted = false;
        }
      }
    }
  
    if (Arduboy2Base::justPressed(UP_BUTTON)) {
      if (!(paused || snakes[0].dead)) {
        if (snakes[0].direction != DOWN) {
          snakes[0].direction = UP;
          snakes[0].control_permitted = false;
        }
      }
    }
  
    if (Arduboy2Base::justPressed(RIGHT_BUTTON)) {
      if (!(paused || snakes[0].dead)) {
        if (snakes[0].direction != LEFT) {
          snakes[0].direction = RIGHT;
          snakes[0].control_permitted = false;
        }
      }
    }
  
    if (Arduboy2Base::justPressed(DOWN_BUTTON)) {
      if (!(paused || snakes[0].dead)) {
        if (snakes[0].direction != UP) {
          snakes[0].direction = DOWN;
          snakes[0].control_permitted = false;
        }
      }
    }
  }
}

void move_snake(int player) {
  struct Snake snake = snakes[player];
  if (snake.dead) {
    return;
  }
  struct Block *chunks = snake.chunks;
  struct Block head = chunks[0];
  
  for (int i = snake.length - 1; i > 0; i--) {
    struct Block current = chunks[i];
    struct Block next = chunks[i - 1];
    current.x = next.x;
    current.y = next.y;
    chunks[i] = current;
  }
  
  switch (snake.direction) {
    case RIGHT:
      head.x++;
      break;
    case DOWN:
      head.y++;
      break;
    case LEFT:
      head.x--;
      break;
    case UP:
      head.y--;
      break;
  }

  if (head.x == MAX_X) {
    head.x = 0;
  } else if (head.x == 255) {
    head.x = MAX_X - 1;
  }

  if (head.y == MAX_Y) {
    head.y = 0;
  } else if (head.y == 255) {
    head.y = MAX_Y - 1;
  }

  chunks[0] = head;
  snake.chunks = chunks;
  snake.control_permitted = true;
  snakes[player] = snake;
}

bool snake_collided(int player) {
  int x = snakes[player].chunks[0].x;
  int y = snakes[player].chunks[0].y;
  for (int i = 0; i < number_of_players; i++) {
    int index = 0;
    if (i == player) {
      index = 1;
    }
    for (int a = index; a < snakes[i].length; a++) {
      struct Block chunk = snakes[i].chunks[a];
      if (chunk.x == x && chunk.y == y) {
        return true;
      }
    }
  }
  return false;
}

bool food_collided(int player) {
  return snakes[player].chunks[0].x == food.x && snakes[player].chunks[0].y == food.y;
}

void eat_food(int player) {
  snakes[player].chunks[snakes[player].length] = {snakes[player].chunks[0].x, snakes[player].chunks[0].y};
  snakes[player].length = snakes[player].length + 1;
  new_food();
  beep1.tone(beep1.freq(BEEP_FREQ), tone_length);
}

void new_food() {
  unsigned int x;
  unsigned int y;
  
  while (true) {
    x = random(0, MAX_X);
    y = random(0, MAX_Y);

    if (!block_occupied(x, y)) {
      break;
    }
  }
  
  food = {x, y};
}

bool block_occupied(int x, int y) {
  for (int i = 0; i < number_of_players; i++) {
    struct Snake snake = snakes[i];
    for (int s = 0; s < snake.length; s++) {
      struct Block chunk = snake.chunks[s];
      if (chunk.x == x && chunk.y == y) {
        return true;
      }
    }
  }
  
  return false;
}

void draw_food() {
  arduboy.fillRect(food.x * BLOCK_WIDTH, food.y * BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, WHITE);
}

void draw_snakes() {
  for (int i = 0; i < number_of_players; i++) {
    for (int a = 0; a < snakes[i].length; a++) {
      struct Block chunk = snakes[i].chunks[a];
      arduboy.fillRect(chunk.x * BLOCK_WIDTH, chunk.y * BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, WHITE);
    }
  }
}

void draw_interface() {
  if (paused) {
    char *pause_graphic = PAUSE_GRAPHIC;
    draw_graphic(pause_graphic, 32, 16, WIDTH / 2, HEIGHT / 2, true, true);
  } else if (all_players_dead()) {
    char *dead_graphic = DEATH_BOX;
    int x = WIDTH / 2;
    int y = HEIGHT / 2;
    draw_graphic(dead_graphic, 80, 23, x, y, true, true);
    arduboy.setCursor(x - 28, y - 9);
    arduboy.print("Score: ");
    int score = 0;
    for (int i = 0; i < number_of_players; i++) {
      int player_score = snakes[i].length;
      if (player_score > score) {
        score = player_score;
      }
    }
    arduboy.print(score);
    arduboy.setCursor(x - 28, y + 2);
    arduboy.print("Speed: ");
    arduboy.print(game_speed);
  }
}

void reset() {  
  for (int i = 0; i < number_of_players; i++) {
    spawn_snake(i);
  }
  
  new_food();
}

void spawn_snake(int player) {
  struct StartConditions start_condition = player_start_conditions[player];
  struct Snake snake = snakes[player];
  
  snake.length = INITIAL_LENGTH;
  snake.direction = start_condition.direction;
  snake.dead = false;
  struct Block *chunks = snake.chunks;
  for (int a = 0; a < snake.length; a++) {
    chunks[a] = {start_condition.x, start_condition.y};
  }

  snake.chunks = chunks;
  snakes[player] = snake;
}

void draw_graphic(char * graphic, int size_x, int size_y, unsigned int x, unsigned int y, bool origin_center, bool blank) {
  char *pause_graphic = PAUSE_GRAPHIC;
  if (origin_center) {
    x = x - size_x / 2;
    y = y - size_y / 2;
  }
  if (blank) {
    arduboy.fillRect(x, y, size_x, size_y, BLACK);
  }
  arduboy.drawBitmap(x, y, graphic, size_x, size_y, WHITE);
}
