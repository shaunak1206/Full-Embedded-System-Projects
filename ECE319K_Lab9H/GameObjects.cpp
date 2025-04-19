#include "GameObjects.h"
#include "../../inc/ST7735.h"
#include "images/images.h"

// GameObject implementation
GameObject::GameObject(int16_t x, int16_t y, uint8_t width, uint8_t height)
    : x(x), y(y), width(width), height(height), active(true) {}

void GameObject::Draw() {
    // Base draw function - empty as drawing is handled by derived objects
}

void GameObject::Update() {
    // Base update function - empty as updating is handled by derived objects
}

// PlayerShip implementation
PlayerShip::PlayerShip()
    : GameObject(64, 120, PLAYER_WIDTH, PLAYER_HEIGHT), lives(3) {}

void PlayerShip::Draw() {
    if (!IsActive()) return;
    // Draw player ship using sprite
    ST7735_DrawBitmap(x, y, PlayerShip0, 18, 8);
}

void PlayerShip::Update() {
    // Player update logic will be handled by input
}

void PlayerShip::Move(int16_t dx) {
    x += dx;
    // Keep player within screen bounds
    if (x < 0) x = 0;
    if (x > 128 - width) x = 128 - width;
}

void PlayerShip::Shoot() {
    // Shooting logic will be handled by GameManager
}

// Alien implementation
Alien::Alien(int16_t x, int16_t y, uint8_t type)
    : GameObject(x, y, ALIEN_WIDTH, ALIEN_HEIGHT), type(type) {}

void Alien::Draw() {
    if (!IsActive()) return;
    // Draw different alien types based on row position
    switch(type) {
        case 0:  // Top row - 30 points
            ST7735_DrawBitmap(x, y, SmallEnemy30pointA, 16, 10);
            break;
        case 1:  // Second row - 20 points
        case 2:
            ST7735_DrawBitmap(x, y, SmallEnemy20pointA, 16, 10);
            break;
        default: // Bottom rows - 10 points
            ST7735_DrawBitmap(x, y, SmallEnemy10pointA, 16, 10);
            break;
    }
}

void Alien::Update() {
    // Alien movement will be handled by GameManager
}

bool Alien::ShouldShoot() const {
    extern uint32_t Random(uint32_t n);
    return (Random(1000) < 2);  // 0.2% chance to shoot each frame
}

// Bullet implementation
Bullet::Bullet(int16_t x, int16_t y, bool isPlayerBullet)
    : GameObject(x, y, BULLET_WIDTH, BULLET_HEIGHT), isPlayerBullet(isPlayerBullet) {}

void Bullet::Draw() {
    if (!IsActive()) return;
    // Draw a simple line for the bullet
    ST7735_DrawFastVLine(x, y, height, 
                        isPlayerBullet ? ST7735_WHITE : ST7735_RED);
}

void Bullet::Update() {
    if (!IsActive()) return;
    // Move bullet up or down based on who shot it
    y += isPlayerBullet ? -2 : 2;
    // Deactivate if out of bounds
    if (y < 0 || y > 160) 
        SetActive(false);
} 