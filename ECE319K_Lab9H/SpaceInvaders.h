#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <stdint.h>
#include <stdbool.h>

// Game constants
#define MAX_BULLETS 5
#define MAX_ENEMY_BULLETS 8
#define ENEMY_ROWS 5
#define ENEMY_COLS 11
#define MAX_PLAYER_LIVES 3

// Point values for different enemy types
#define POINTS_ROW_1 30    // Top row
#define POINTS_ROW_2_3 20  // Middle rows
#define POINTS_ROW_4_5 10  // Bottom rows

// Game states
typedef enum {
    MENU_STATE,
    GAME_STATE,
    GAME_OVER_STATE
} GameState;

// Language options
typedef enum {
    ENGLISH,
    SPANISH,
    PORTUGUESE,
    FRENCH,
    NUM_LANGUAGES
} Language;

// Enemy types
typedef enum {
    ENEMY_TYPE_30_POINTS,  // Top row
    ENEMY_TYPE_20_POINTS,  // Middle rows
    ENEMY_TYPE_10_POINTS   // Bottom rows
} EnemyType;

// Enemy structure
typedef struct {
    int16_t x, y;
    bool active;
    EnemyType type;
} Enemy;

// Bullet structure
typedef struct {
    int16_t x, y;
    bool active;
    bool isEnemy;
} Bullet;

// Game variables
extern uint32_t gameScore;
extern uint8_t playerLives;
extern Enemy enemies[ENEMY_ROWS][ENEMY_COLS];
extern Bullet enemyBullets[MAX_ENEMY_BULLETS];
extern bool enemiesMovingRight;

// Random number generator
extern uint32_t Random(uint32_t);

// Game functions
void Game_Init(void);
void Game_Update(void);
void Game_Draw(void);

// Menu functions
void Menu_Update(void);
void Menu_Draw(void);

// Helper functions
void FireBullet(void);
uint32_t GetPointsForEnemy(uint8_t type);
void CheckCollisions(void);

#endif // SPACEINVADERS_H 