#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <stdint.h>
#include "GameObjects.h"

#define MAX_ALIENS 55  // 11 columns * 5 rows
#define MAX_BULLETS 10

class GameManager {
private:
    PlayerShip player;
    Alien* aliens[MAX_ALIENS];
    Bullet* bullets[MAX_BULLETS];
    uint16_t score;
    uint8_t gameOver;

    void InitAliens();
    void UpdateAliens();
    void UpdateBullets();
    void CheckCollisions();
    void DrawScore();

public:
    GameManager();  // Constructor
    void Init();
    void Update();
    void Draw();
    uint8_t IsGameOver() const { return gameOver; }
    uint16_t GetScore() const { return score; }
};

#endif // GAMEMANAGER_H 