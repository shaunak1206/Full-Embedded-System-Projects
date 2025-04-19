#include "GameManager.h"
#include "Switch.h"
#include "../inc/ST7735.h"

GameManager::GameManager() : score(0), gameOver(false) {
    // Initialize alien array
    for (uint8_t i = 0; i < MAX_ALIENS; i++) {
        aliens[i] = nullptr;
    }
    // Initialize bullet array
    for (uint8_t i = 0; i < MAX_BULLETS; i++) {
        bullets[i] = nullptr;
    }
}

void GameManager::Init() {
    // Clear screen
    ST7735_FillScreen(ST7735_BLACK);
    // Initialize aliens
    InitAliens();
    // Draw initial score
    DrawScore();
}

void GameManager::InitAliens() {
    uint8_t alienIndex = 0;
    // Create 5 rows of aliens
    for (uint8_t row = 0; row < 5; row++) {
        for (uint8_t col = 0; col < 11; col++) {
            if (alienIndex < MAX_ALIENS) {
                aliens[alienIndex] = new Alien(col * 10 + 4, row * 10 + 4, row);
                alienIndex++;
            }
        }
    }
}

void GameManager::Update() {
    // Handle player input
    uint32_t switchState = Switch_In();
    if (switchState & 0x01) {  // Switch 1 (PB12) - Left
        player.Move(-2);
    }
    if (switchState & 0x02) {  // Switch 2 (PB16) - Right
        player.Move(2);
    }

    // Update game objects
    UpdateAliens();
    UpdateBullets();
    CheckCollisions();
}

void GameManager::UpdateAliens() {
    static int16_t dx = 1;  // Move static variable outside the loop
    for (uint8_t i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i] && aliens[i]->IsActive()) {
            aliens[i]->Update();
            aliens[i]->Move(dx, 0);
            
            // Check if aliens should change direction
            if (aliens[i]->GetX() >= 128 - ALIEN_WIDTH || aliens[i]->GetX() <= 0) {
                dx = -dx;
                // Move all aliens down
                for (uint8_t j = 0; j < MAX_ALIENS; j++) {
                    if (aliens[j] && aliens[j]->IsActive()) {
                        aliens[j]->Move(0, 5);
                    }
                }
                break;
            }
        }
    }
}

void GameManager::UpdateBullets() {
    for (uint8_t i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i] && bullets[i]->IsActive()) {
            bullets[i]->Update();
        }
    }
}

void GameManager::CheckCollisions() {
    // TODO: Implement collision detection
}

void GameManager::Draw() {
    // Clear screen
    ST7735_FillScreen(ST7735_BLACK);
    
    // Draw game objects
    player.Draw();
    for (uint8_t i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i] && aliens[i]->IsActive()) {
            aliens[i]->Draw();
        }
    }
    for (uint8_t i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i] && bullets[i]->IsActive()) {
            bullets[i]->Draw();
        }
    }
    
    // Draw score
    DrawScore();
}

void GameManager::DrawScore() {
    const char* scoreText = "Score: ";
    ST7735_SetCursor(0, 0);
    ST7735_OutString((char*)scoreText);
    ST7735_OutUDec(score);
} 