#pragma once

#include "GameServer.hpp"

Server::Server(uint16_t port)
	: socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
	rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()))
{
	std::cout << "UDP server listening on port: " << port << "\n";
}

void Server::Run()
{
	while (true)
	{
		char data[1024];
		asio::ip::udp::endpoint clientEndpoint;
		asio::error_code ec;

		size_t len = socket.receive_from(asio::buffer(data), clientEndpoint, 0, ec);
		if (ec)
		{
			std::cerr << "Reception error: " << ec.message() << std::endl;
			continue;
		}

		if (len < sizeof(PlayerData)) continue;

		PlayerData receivedPlayer;
		std::memcpy(&receivedPlayer, data, sizeof(PlayerData));

		uint32_t playerId;

		if (clients.find(clientEndpoint) == clients.end())
		{
			playerId = nextId++;
			PlayerData newPlayer;
			newPlayer.id = playerId;
			newPlayer.x = receivedPlayer.x;
			newPlayer.y = receivedPlayer.y;
			newPlayer.size = receivedPlayer.size;

			std::uniform_int_distribution<int> dist(50, 255);
			newPlayer.r = dist(rng);
			newPlayer.g = dist(rng);
			newPlayer.b = dist(rng);

			clients[clientEndpoint] = newPlayer;

			std::cout << "Nuovo player: ID " << playerId
				<< " da " << clientEndpoint.address().to_string()
				<< ":" << clientEndpoint.port() << std::endl;

			socket.send_to(asio::buffer(&newPlayer, sizeof(PlayerData)), clientEndpoint);
		}
		else
		{
			playerId = clients[clientEndpoint].id;
			clients[clientEndpoint].x = receivedPlayer.x;
			clients[clientEndpoint].size = receivedPlayer.size;
			clients[clientEndpoint].y = receivedPlayer.y;
		}

		std::vector<PlayerData> playerList;
		for (const auto& [_, player] : clients)
			playerList.push_back(player);
		
		for (const auto& [endpoint, _] : clients)
		{
			asio::error_code ec;
			socket.send_to(asio::buffer(playerList.data(), playerList.size() * sizeof(PlayerData)), endpoint, 0, ec);
			
			if (ec)
				std::cerr << "Errore invio a " << endpoint.address().to_string()
				<< ": " << ec.message() << std::endl;
		}
	}
}
