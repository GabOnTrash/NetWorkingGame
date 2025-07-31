#pragma once

#include "Headers.hpp"

class Server
{
public:

	Server(uint16_t port);
	void Run();

private:

	asio::io_context context;
	asio::ip::udp::socket socket;
	std::unordered_map<asio::ip::udp::endpoint, PlayerData> clients;
	uint32_t nextId = 1;

	std::mt19937 rng;
};