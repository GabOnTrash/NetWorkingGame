#pragma once

#include "Headers.hpp"

class Game
{
public:
	
	Game();
	void Run();

private:

	void GetPlayerInput(PlayerData& player);
	void DrawPlayers(const PlayerData& data);
	void GetOppDataFromServer();
	void SendPlayerDataToServer();
	void StopConnection();

	std::unordered_map<uint32_t, PlayerData> players;
	PlayerData localPlayer;

	GLFWwindow* window;

	asio::io_context context;
	std::unique_ptr<asio::ip::udp::socket> socket;
	asio::ip::udp::endpoint endpoint;
	std::mutex playersMutex;
	std::thread receiveThread;

	std::mt19937 rng;
};