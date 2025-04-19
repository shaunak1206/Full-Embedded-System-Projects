#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <stdint.h>
#include "../../inc/ST7735.h"

// Constants for game objects
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 8
#define ALIEN_WIDTH 16
#define ALIEN_HEIGHT 8
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 4

// Base class for all game objects
class GameObject {
protected:
    int16_t x, y;
    uint8_t width, height;
    bool active;

public:
    GameObject(int16_t x, int16_t y, uint8_t width, uint8_t height);
    virtual ~GameObject() = default;
    virtual void Draw();
    virtual void Update();
    bool IsActive() const { return active; }
    void SetActive(bool state) { active = state; }
    int16_t GetX() const { return x; }
    int16_t GetY() const { return y; }
    uint8_t GetWidth() const { return width; }
    uint8_t GetHeight() const { return height; }
    virtual void Move(int16_t dx, int16_t dy) { x += dx; y += dy; }
};

// Player ship class
class PlayerShip : public GameObject {
private:
    uint8_t lives;

public:
    PlayerShip();
    void Draw() override;
    void Update() override;
    void Move(int16_t dx);  // Override with single parameter version
    void Shoot();
    uint8_t GetLives() const { return lives; }
    void LoseLife() { if (lives > 0) lives--; }
};

// Alien class
class Alien : public GameObject {
private:
    uint8_t type;

public:
    Alien(int16_t x, int16_t y, uint8_t type);
    void Draw() override;
    void Update() override;
    bool ShouldShoot() const;
    using GameObject::Move;  // Inherit the Move function from GameObject
};

// Bullet class
class Bullet : public GameObject {
private:
    bool isPlayerBullet;

public:
    Bullet(int16_t x, int16_t y, bool isPlayerBullet);
    void Draw() override;
    void Update() override;
    bool IsPlayerBullet() const { return isPlayerBullet; }
};

#endif // GAMEOBJECTS_H 