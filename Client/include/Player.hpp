#pragma once

#include <GLFW/glfw3.h>

#pragma pack(push, 1)
struct PlayerData
{
    uint8_t r = 255, g = 255, b = 255;
    uint32_t id;
    float x = 0.0f;
    float y = 0.0f;
    float size = 0.2f;
};
#pragma pack(pop)

class Player
{
public:

    Player() {};
    Player(GLFWwindow* window);

    void Draw();
    void Update();
    void Input();

    PlayerData data;

private:

    GLFWwindow* window;
};