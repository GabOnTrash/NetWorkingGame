#include "Player.hpp"

Player::Player(GLFWwindow* window)
    : window(window)
{

}
void Player::Update()
{
    Input();
}
void Player::Input()
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    data.y += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  data.y -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  data.x -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) data.x += 0.01f;
}
void Player::Draw()
{
    glColor3ub(data.r, data.g, data.b);
    glBegin(GL_QUADS);
    glVertex2f(data.x - data.size / 2.0f, data.y - data.size / 2.0f);
    glVertex2f(data.x + data.size / 2.0f, data.y - data.size / 2.0f);
    glVertex2f(data.x + data.size / 2.0f, data.y + data.size / 2.0f);
    glVertex2f(data.x - data.size / 2.0f, data.y + data.size / 2.0f);
    glEnd();
}