#include <Arduboy2.h>

#define BLOCK_WIDTH 4
#define MAX_X (WIDTH / BLOCK_WIDTH)
#define MAX_Y (HEIGHT / BLOCK_WIDTH)
#define MAX_SNAKE (MAX_X * MAX_Y)
#define INITIAL_LENGTH 3
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

struct block {
  int x;
  int y;
};

struct snake {
  struct block *chunks;
  unsigned int length;
};

const unsigned char PAUSE_GRAPHIC[] PROGMEM = {0xff, 0x3, 0x1, 0x1, 0xe1, 0xa1, 0xe1, 0x1, 0xe1, 0xa1, 0xe1, 0x1, 0xe1, 0x1, 0xe1, 0x1, 0xe1, 0xa1, 0xa1, 0x1, 0xe1, 0xa1, 0xa1, 0x1, 0xe1, 0x21, 0x21, 0xc1, 0x1, 0x1, 0x3, 0xff, 0xff, 0xc0, 0x80, 0x80, 0x87, 0x80, 0x80, 0x80, 0x87, 0x80, 0x87, 0x80, 0x87, 0x84, 0x87, 0x80, 0x84, 0x84, 0x87, 0x80, 0x87, 0x84, 0x84, 0x80, 0x87, 0x84, 0x84, 0x83, 0x80, 0x80, 0xc0, 0xff};

Arduboy2 arduboy;
struct snake snake;
struct block food;
unsigned int direction = RIGHT;
bool paused = false;
unsigned char last_input = 0x00;

void setup() {
  arduboy.boot();
  arduboy.setFrameRate(20);
  arduboy.initRandomSeed();
  snake.chunks = new struct block[255];
  snake.length = INITIAL_LENGTH;
  for (int i = 0; i < snake.length; i++) {
    snake.chunks[i] = {0, 0};
  }
  new_food();
}

void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }
  
  process_input();

  if (!paused) {
    move_snake();
    if (snake_collided()) {
      lose();
    }
    if (food_collided()) {
      eat_food();
    }
  }
  
  arduboy.clear();
  draw_food();
  draw_snake();
  draw_interface();
  arduboy.display();
}

void process_input() {
  unsigned char current_input = arduboy.buttonsState();
  if (current_input != last_input) {
    last_input = current_input;
  } else {
    return;
  }
  
  if (arduboy.pressed(A_BUTTON)) {
    paused = !paused;
  }

  if (paused) {
    return;
  }
  
  if (direction != RIGHT && arduboy.pressed(LEFT_BUTTON)) {
     direction = LEFT;
  }
  if (direction != UP && arduboy.pressed(DOWN_BUTTON)) {
     direction = DOWN;
  }
  if (direction != LEFT && arduboy.pressed(RIGHT_BUTTON)) {
     direction = RIGHT;
  }
  if (direction != DOWN && arduboy.pressed(UP_BUTTON)) {
     direction = UP;
  }
}

void move_snake() {
  for (int i = snake.length - 1; i > 0; i--) {
    snake.chunks[i].x = snake.chunks[i - 1].x;
    snake.chunks[i].y = snake.chunks[i - 1].y;
  }
  
  switch (direction) {
    case RIGHT:
      snake.chunks[0].x = snake.chunks[0].x + 1;
      break;
     case DOWN:
      snake.chunks[0].y = snake.chunks[0].y + 1;
      break;
     case LEFT:
      snake.chunks[0].x = snake.chunks[0].x - 1;
      break;
     case UP:
      snake.chunks[0].y = snake.chunks[0].y - 1;
      break;
  }

  if (snake.chunks[0].x >= MAX_X) {
    snake.chunks[0].x = 0;
  } else if (snake.chunks[0].x < 0) {
    snake.chunks[0].x = MAX_X - 1;
  }

  if (snake.chunks[0].y >= MAX_Y) {
    snake.chunks[0].y = 0;
  } else if (snake.chunks[0].y < 0) {
    snake.chunks[0].y = MAX_Y - 1;
  }
}

bool snake_collided() {
  int x = snake.chunks[0].x;
  int y = snake.chunks[0].y;
  for (int i = 1; i < snake.length; i++) {
    if (snake.chunks[i].x == x && snake.chunks[i].y == y) {
      return true;
    }
  }
  return false;
}

bool food_collided() {
  return snake.chunks[0].x == food.x && snake.chunks[0].y == food.y;
}

void eat_food() {
  snake.chunks[snake.length] = {snake.chunks[0].x, snake.chunks[0].y};
  snake.length = snake.length + 1;
  new_food();
}

void new_food() {
  unsigned int x = random(0, MAX_X);
  unsigned int y = random(0, MAX_Y);
  food = {x, y};
}

void draw_food() {
  arduboy.fillRect(food.x * BLOCK_WIDTH, food.y * BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, WHITE);
}

void draw_snake() {
  for (int i = 0; i < snake.length; i++) {
    struct block chunk = snake.chunks[i];
    arduboy.fillRect(chunk.x * BLOCK_WIDTH, chunk.y * BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, WHITE);
  }
}

void draw_interface() {
  if (paused) {
    char *PAUSE_SPRITE = PAUSE_GRAPHIC;
    unsigned int x = WIDTH / 2 - 16;
    unsigned int y = HEIGHT / 2 - 8;
    arduboy.fillRect(x, y, 32, 16, BLACK);
    arduboy.drawBitmap(x, y, PAUSE_SPRITE, 32, 16, WHITE);
  }
}

void lose() {
  snake.length = INITIAL_LENGTH;
}
