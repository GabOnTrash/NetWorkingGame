#pragma once

#define ASIO_STANDALONE
#define _WIN32_WINNT 0x0601 // O superiore se usi Windows 10+
#include <asio.hpp>
#include <GLFW/glfw3.h>

#include <mutex>
#include <thread>
#include <memory>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>
#include <unordered_map>

#include "Player.hpp"