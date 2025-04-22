#include "SpaceInvaders.h"
#include "../../inc/ST7735.h"
#include "../../inc/SlidePot.h"
#include "Sound.h"
#include "Switch.h"
#include "images/images.h"
#include <string.h>
#include <stdio.h>

// Game state management
static GameState currentState = MENU_STATE;
static Language currentLanguage = ENGLISH;

// Ship and enemy dimensions
static int16_t ShipX = 80;    // Start in middle of screen (160/2)
static const int16_t ShipY = 110;  // Near bottom of screen in landscape
static const uint8_t ShipWidth = 18;
static const uint8_t ShipHeight = 8;
static const uint8_t EnemyWidth = 16;
static const uint8_t EnemyHeight = 10;

// Screen dimensions for landscape mode
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
#define CHAR_WIDTH 6    // Width of each character
#define MAX_CHARS_PER_LINE (SCREEN_WIDTH/CHAR_WIDTH - 1)  // Leave 1 char margin

// Game variables
uint32_t gameScore = 0;
uint8_t playerLives = MAX_PLAYER_LIVES;
Enemy enemies[ENEMY_ROWS][ENEMY_COLS];
Bullet enemyBullets[MAX_ENEMY_BULLETS];
bool enemiesMovingRight = true;
static int16_t enemyBlockX = 0;  // X offset for entire enemy block
static int16_t enemyBlockY = 10; // Y offset for entire enemy block

// Bullet management
static Bullet bullets[MAX_BULLETS];
static uint32_t lastButtonState = 0;

// Language strings
const char* TITLE[] = {"SPACE INVADERS!", "INVASORES!", "INVASORES!", "ENVAHISSEURS!"};
const char* AUTHOR = "Manas Pathak";
const char* START_TEXT[] = {"Press S1", "Presiona S1", "Pressione S1", "Appuyez S1"};
const char* LANGUAGE_TEXT[] = {"English", "Espanol", "Portugues", "Francais"};
const char* GAME_OVER_TEXT[] = {"Game Over!", "Juego Terminado!", "Fim de Jogo!", "Jeu Termine!"};
const char* WIN_TEXT[] = {"You Win!", "Ganaste!", "Voce Venceu!", "Vous Gagnez!"};
const char* SCORE_TEXT[] = {"Score:", "Puntaje:", "Pontos:", "Score:"};
const char* SLIDE_SELECT[] = {"Slide to Select", "Deslizar", "Deslize", "Glisser"};
const char* PLAY_AGAIN_TEXT[] = {"Press S1", "Presiona S1", "Pressione S1", "Appuyez S1"};
const char* LIVES_TEXT[] = {"Lives:", "Vidas:", "Vidas:", "Vies:"};

// Declare external SlidePot object
extern SlidePot Sensor;

// Add at the top with other static variables
static uint32_t moveCounter = 0;  // Counter for slowing enemy movement
static uint32_t animationCounter = 0;  // For sprite animation
static bool useAlternateSprite = false;  // Toggle between A and B sprites

// Constants
#define MAX_BULLETS 5      // Maximum number of player bullets
#define MAX_ENEMY_BULLETS 8 // Maximum number of enemy bullets
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 4
#define BULLET_SPEED 2     // Pixels per frame
#define ENEMY_ROWS 4      // Reduced from 5 to 4 rows
#define ENEMY_COLS 8      // Reduced from 11 to 8 columns
#define ENEMY_SPACING_X 16 // Horizontal space between enemies
#define ENEMY_SPACING_Y 16 // Vertical space between enemies
#define ENEMY_MOVE_SPEED 0.25 // Slower horizontal movement (moves every 4 frames)
#define ENEMY_DROP_SPEED 2 // Much slower vertical drop
#define MAX_PLAYER_LIVES 3
#define ENEMY_SHOOT_CHANCE 20 // Reduced shooting frequency (1 in 20 chance per enemy per frame)
#define ENEMY_MARGIN 8    // Margin from screen edges

// Initialize enemies in their starting positions
void InitEnemies(void) {
    // Calculate total width of enemy block
    int16_t totalEnemyWidth = ENEMY_COLS * (EnemyWidth + 4);  // 4 pixels between enemies
    // Calculate starting X to center the enemy block
    int16_t startX = (SCREEN_WIDTH - totalEnemyWidth) / 2;
    
    for(int row = 0; row < ENEMY_ROWS; row++) {
        for(int col = 0; col < ENEMY_COLS; col++) {
            enemies[row][col].x = col * (EnemyWidth + 4);  // 4 pixels between enemies
            enemies[row][col].y = row * (EnemyHeight + 6); // 6 pixels between rows
            enemies[row][col].active = true;
            enemies[row][col].type = (row < 1) ? ENEMY_TYPE_30_POINTS :
                                    (row < 2) ? ENEMY_TYPE_20_POINTS :
                                               ENEMY_TYPE_10_POINTS;
        }
    }
    
    // Start enemies centered horizontally with margin from left
    enemyBlockX = startX;
    enemyBlockY = 20;  // Start a bit lower from top
    enemiesMovingRight = true;
    
    // Clear enemy bullets
    for(int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBullets[i].active = false;
    }
}

// Initialize hardware and game state
void Game_Init(void) {
    // Initialize display in landscape mode
    ST7735_InitPrintf(INITR_REDTAB);
    ST7735_SetRotation(3);  // Changed from 1 to 3 to flip right-side up
    ST7735_FillScreen(ST7735_BLACK);
    
    // Initialize hardware
    Sensor.Init();
    Switch_Init();
    Sound_Init();
    
    // Initialize game variables
    gameScore = 0;
    playerLives = MAX_PLAYER_LIVES;
    
    // Initialize bullets
    for(int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
    
    // Initialize enemies
    InitEnemies();
    
    // Start in menu state
    currentState = MENU_STATE;
}

// Fire an enemy bullet
void FireEnemyBullet(int16_t x, int16_t y) {
    for(int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if(!enemyBullets[i].active) {
            enemyBullets[i].x = x;
            enemyBullets[i].y = y;
            enemyBullets[i].active = true;
            enemyBullets[i].isEnemy = true;
            Sound_Shoot();
            break;
        }
    }
}

// Update enemy positions and state
void UpdateEnemies(void) {
    bool shouldDrop = false;
    bool enemiesAlive = false;
    
    // Slow down enemy movement (move every 8 frames)
    moveCounter = (moveCounter + 1) % 8;
    
    // Update animation every 16 frames
    animationCounter = (animationCounter + 1) % 16;
    if(animationCounter == 0) {
        useAlternateSprite = !useAlternateSprite;
    }
    
    if(moveCounter != 0) {
        return;
    }
    
    // Play background music when enemies move
    Sound_Background();
    
    // Check boundaries and update movement
    for(int row = 0; row < ENEMY_ROWS; row++) {
        for(int col = 0; col < ENEMY_COLS; col++) {
            if(!enemies[row][col].active) continue;
            
            enemiesAlive = true;
            int16_t actualX = enemies[row][col].x + enemyBlockX;
            
            // Check screen boundaries with margin
            if(enemiesMovingRight && actualX >= (SCREEN_WIDTH - EnemyWidth - ENEMY_MARGIN)) {
                shouldDrop = true;
                enemiesMovingRight = false;
                break;
            } else if(!enemiesMovingRight && actualX <= ENEMY_MARGIN) {
                shouldDrop = true;
                enemiesMovingRight = true;
                break;
            }
            
            // Check if enemies reached bottom
            int16_t actualY = enemies[row][col].y + enemyBlockY;
            if(actualY >= (ShipY - EnemyHeight)) {
                playerLives = 0;
                currentState = GAME_OVER_STATE;
                return;
            }
            
            // Random chance to shoot
            if(Random(1000) < ENEMY_SHOOT_CHANCE) {
                FireEnemyBullet(actualX + EnemyWidth/2, actualY + EnemyHeight);
            }
        }
        if(shouldDrop) break;
    }
    
    // If no enemies are alive, player wins
    if(!enemiesAlive) {
        currentState = GAME_OVER_STATE;
        return;
    }
    
    // Move enemies
    if(shouldDrop) {
        enemyBlockY += ENEMY_DROP_SPEED;
    } else {
        enemyBlockX += enemiesMovingRight ? 1 : -1;
    }
}

// Update game state based on input
void Game_Update(void) {
    uint32_t currentButtonState = Switch_In();
    
    if(currentState == MENU_STATE) {
        Menu_Update();
        return;
    } else if(currentState == GAME_OVER_STATE) {
        // Check for restart
        if((currentButtonState & 0x01) && !(lastButtonState & 0x01)) {
            Game_Init();  // Reset game
            return;
        }
    } else {  // GAME_STATE
        // Check for pause (SW2)
        if((currentButtonState & 0x02) && !(lastButtonState & 0x02)) {
            currentState = MENU_STATE;
            ST7735_FillScreen(ST7735_BLACK);
            return;
        }
        
        // Update ship position for landscape mode
        uint32_t adc_value = Sensor.In();
        Sensor.Save(adc_value);
        ShipX = ((adc_value * (SCREEN_WIDTH-ShipWidth)) >> 12);
        
        // Fire bullet (SW1)
        if((currentButtonState & 0x01) && !(lastButtonState & 0x01)) {
            FireBullet();
        }
        
        // Update game objects
        UpdateEnemies();
        
        // Update player bullets
        for(int i = 0; i < MAX_BULLETS; i++) {
            if(bullets[i].active) {
                bullets[i].y -= BULLET_SPEED;
                if(bullets[i].y < 0) bullets[i].active = false;
            }
        }
        
        // Update enemy bullets
        for(int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if(enemyBullets[i].active) {
                enemyBullets[i].y += BULLET_SPEED;
                if(enemyBullets[i].y > SCREEN_HEIGHT) enemyBullets[i].active = false;
            }
        }
        
        // Check collisions
        CheckCollisions();
    }
    
    lastButtonState = currentButtonState;
}

// Update the CheckCollisions function to remove bunker collisions
void CheckCollisions(void) {
    // Check player bullet collisions with enemies
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(!bullets[i].active) continue;
        
        // Check collision with enemies
        for(int row = 0; row < ENEMY_ROWS; row++) {
            for(int col = 0; col < ENEMY_COLS; col++) {
                if(!enemies[row][col].active) continue;
                
                int16_t enemyX = enemies[row][col].x + enemyBlockX;
                int16_t enemyY = enemies[row][col].y + enemyBlockY;
                
                if(bullets[i].x >= enemyX && bullets[i].x < enemyX + ShipWidth &&
                   bullets[i].y >= enemyY && bullets[i].y < enemyY + ShipHeight) {
                    // Hit an enemy
                    gameScore += GetPointsForEnemy(enemies[row][col].type);
                    enemies[row][col].active = false;
                    bullets[i].active = false;
                    Sound_Killed();
                    break;
                }
            }
        }
    }
    
    // Check enemy bullet collisions
    for(int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if(!enemyBullets[i].active) continue;
        
        // Check collision with player
        if(enemyBullets[i].x >= ShipX && enemyBullets[i].x < ShipX + ShipWidth &&
           enemyBullets[i].y >= ShipY && enemyBullets[i].y < ShipY + ShipHeight) {
            playerLives--;
            enemyBullets[i].active = false;
            Sound_Explosion();
            if(playerLives == 0) {
                currentState = GAME_OVER_STATE;
            }
            break;
        }
    }
    
    // Check if any enemy has reached the bottom or touched the player
    for(int row = 0; row < ENEMY_ROWS; row++) {
        for(int col = 0; col < ENEMY_COLS; col++) {
            if(!enemies[row][col].active) continue;
            
            int16_t enemyY = enemies[row][col].y + enemyBlockY;
            if(enemyY >= ShipY) {  // Enemy reached player level
                playerLives = 0;
                currentState = GAME_OVER_STATE;
                return;
            }
        }
    }
}

// Draw game over screen with centered text
void DrawGameOver(void) {
    ST7735_FillScreen(ST7735_BLACK);
    
    // Draw game over or win message (centered, moved up)
    char* message = (char*)(playerLives > 0 ? WIN_TEXT[currentLanguage] : GAME_OVER_TEXT[currentLanguage]);
    uint8_t messageLength = strlen(message);
    uint8_t messageX = (SCREEN_WIDTH - (messageLength * 6)) / 2;
    ST7735_SetCursor(messageX/6, 2);  // Moved up to match menu positioning
    ST7735_SetTextColor(playerLives > 0 ? ST7735_GREEN : ST7735_RED);
    ST7735_OutString(message);
    
    // Draw score (centered)
    char scoreBuffer[20];
    sprintf(scoreBuffer, "%s %d", SCORE_TEXT[currentLanguage], gameScore);
    uint8_t scoreLength = strlen(scoreBuffer);
    uint8_t scoreX = (SCREEN_WIDTH - (scoreLength * 6)) / 2;
    ST7735_SetCursor(scoreX/6, 4);  // Moved up
    ST7735_SetTextColor(ST7735_YELLOW);
    ST7735_OutString(scoreBuffer);
    
    // Draw play instruction (centered)
    char* playMessage = (char*)PLAY_AGAIN_TEXT[currentLanguage];
    uint8_t playLength = strlen(playMessage);
    uint8_t playX = (SCREEN_WIDTH - (playLength * 6)) / 2;
    ST7735_SetCursor(playX/6, 6);  // Moved up
    ST7735_SetTextColor(ST7735_CYAN);
    ST7735_OutString(playMessage);
}

// Draw current game state
void Game_Draw(void) {
    if(currentState == MENU_STATE) {
        Menu_Draw();
    } else if(currentState == GAME_OVER_STATE) {
        DrawGameOver();
    } else {
        ST7735_FillScreen(ST7735_BLACK);
        
        // Draw player ship
        ST7735_DrawBitmap(ShipX, ShipY, PlayerShip0, ShipWidth, ShipHeight);
        
        // Draw enemies with animation
        for(int row = 0; row < ENEMY_ROWS; row++) {
            for(int col = 0; col < ENEMY_COLS; col++) {
                if(!enemies[row][col].active) continue;
                
                int16_t x = enemies[row][col].x + enemyBlockX;
                int16_t y = enemies[row][col].y + enemyBlockY;
                
                const uint16_t* sprite;
                switch(enemies[row][col].type) {
                    case ENEMY_TYPE_30_POINTS:
                        sprite = useAlternateSprite ? SmallEnemy30pointB : SmallEnemy30pointA;
                        break;
                    case ENEMY_TYPE_20_POINTS:
                        sprite = useAlternateSprite ? SmallEnemy20pointB : SmallEnemy20pointA;
                        break;
                    default:
                        sprite = useAlternateSprite ? SmallEnemy10pointB : SmallEnemy10pointA;
                        break;
                }
                ST7735_DrawBitmap(x, y, sprite, EnemyWidth, EnemyHeight);
            }
        }
        
        // Draw bullets
        for(int i = 0; i < MAX_BULLETS; i++) {
            if(bullets[i].active) {
                ST7735_DrawFastVLine(bullets[i].x, bullets[i].y, BULLET_HEIGHT, ST7735_WHITE);
            }
        }
        
        for(int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if(enemyBullets[i].active) {
                ST7735_DrawFastVLine(enemyBullets[i].x, enemyBullets[i].y, BULLET_HEIGHT, ST7735_RED);
            }
        }
        
        // Draw score at left
        ST7735_SetCursor(0, 0);
        ST7735_SetTextColor(ST7735_YELLOW);
        ST7735_OutString((char*)SCORE_TEXT[currentLanguage]);
        ST7735_OutString(" ");
        char scoreStr[10];
        sprintf(scoreStr, "%d", gameScore);
        ST7735_OutString(scoreStr);
        
        // Lives counter moved more to the left (position 13 instead of 15)
        ST7735_SetCursor(11, 0);
        ST7735_SetTextColor(ST7735_RED);
        char livesBuffer[10];
        sprintf(livesBuffer, "%s %d", LIVES_TEXT[currentLanguage], playerLives);
        ST7735_OutString(livesBuffer);
    }
}

// Draw menu screen
void Menu_Draw(void) {
    ST7735_FillScreen(ST7735_BLACK);
    
    // Calculate maximum width available for text
    const uint8_t maxWidth = MAX_CHARS_PER_LINE * CHAR_WIDTH;
    
    // Draw title in orange (centered)
    char* titleText = (char*)TITLE[currentLanguage];
    uint8_t titleLength = strlen(titleText);
    uint8_t titleX = (titleLength * CHAR_WIDTH <= maxWidth) ? 
                     (SCREEN_WIDTH - (titleLength * CHAR_WIDTH)) / 2 / CHAR_WIDTH :
                     1;  // If too long, align left with margin
    ST7735_SetCursor(titleX, 2);
    ST7735_SetTextColor(ST7735_ORANGE);
    ST7735_OutString(titleText);
    
    // Draw author in yellow (centered)
    char* authorText = (char*)AUTHOR;
    uint8_t authorLength = strlen(authorText);
    uint8_t authorX = (SCREEN_WIDTH - (authorLength * CHAR_WIDTH)) / 2 / CHAR_WIDTH;
    ST7735_SetCursor(authorX, 4);
    ST7735_SetTextColor(ST7735_YELLOW);
    ST7735_OutString(authorText);
    
    // Draw "Slide to Select" in purple (centered)
    char* slideText = (char*)SLIDE_SELECT[currentLanguage];
    uint8_t slideLength = strlen(slideText);
    uint8_t slideX = (SCREEN_WIDTH - (slideLength * CHAR_WIDTH)) / 2 / CHAR_WIDTH;
    ST7735_SetCursor(slideX, 6);
    ST7735_SetTextColor(ST7735_MAGENTA);
    ST7735_OutString(slideText);
    
    // Draw language selection (centered)
    char* langText = (char*)LANGUAGE_TEXT[currentLanguage];
    uint8_t langLength = strlen(langText);
    uint8_t langX = (SCREEN_WIDTH - (langLength * CHAR_WIDTH)) / 2 / CHAR_WIDTH;
    ST7735_SetCursor(langX, 8);
    ST7735_OutString(langText);
    
    // Draw start text in yellow (centered)
    char* startText = (char*)START_TEXT[currentLanguage];
    uint8_t startLength = strlen(startText);
    uint8_t startX = (SCREEN_WIDTH - (startLength * CHAR_WIDTH)) / 2 / CHAR_WIDTH;
    ST7735_SetCursor(startX, 10);
    ST7735_SetTextColor(ST7735_YELLOW);
    ST7735_OutString(startText);
}

// Update menu state
void Menu_Update(void) {
    uint32_t adc_value = Sensor.In();
    Sensor.Save(adc_value);
    
    // Map ADC value to language selection (0-4095 to 0-3)
    uint32_t section = adc_value >> 10;  // Divide by 1024 (4096/4)
    if(section >= NUM_LANGUAGES) {
        section = NUM_LANGUAGES - 1;
    }
    currentLanguage = (Language)section;
    
    // Check for button presses
    uint32_t currentButtonState = Switch_In();
    if((currentButtonState & 0x01) && !(lastButtonState & 0x01)) {
        // SW1 pressed - start game
        currentState = GAME_STATE;
        ST7735_FillScreen(ST7735_BLACK);  // Clear screen before starting
        Sound_Shoot();  // Play sound when starting game
    }
    lastButtonState = currentButtonState;
}

// Fire a new bullet from the ship's position
void FireBullet(void) {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(!bullets[i].active) {
            bullets[i].x = ShipX + (ShipWidth/2) - (BULLET_WIDTH/2);
            bullets[i].y = ShipY - BULLET_HEIGHT;
            bullets[i].active = true;
            Sound_Shoot();  // Play shoot sound
            break;
        }
    }
}

// Get points for different enemy types
uint32_t GetPointsForEnemy(uint8_t type) {
    switch(type) {
        case 0: return POINTS_ROW_1;    // Top row (30 points)
        case 1: return POINTS_ROW_2_3;  // Middle rows (20 points)
        default: return POINTS_ROW_4_5; // Bottom rows (10 points)
    }
} 