#include "Game.hpp"

Game::Game()
{
    socket = std::make_unique<asio::ip::udp::socket>(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
    endpoint = asio::ip::udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 9000);

    receiveThread = std::thread(&Game::GetOppDataFromServer, this);

    if (!glfwInit())
    {
        std::cerr << "Errore inizializzazione GLFW\n";
        return;
    }

    window = glfwCreateWindow(1000, 1000, "NetWorkingGame-ClientSide", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glOrtho(-1, 1, -1, 1, -1, 1);

    localPlayer.x = 0.f;
    localPlayer.y = 0.f;
    localPlayer.size = 0.1f;
    localPlayer.r = 255;
    localPlayer.g = 255;
    localPlayer.b = 255;
    localPlayer.id = 0;
}
void Game::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    localPlayer.y += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  localPlayer.y -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  localPlayer.x -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) localPlayer.x += 0.01f;

        DrawPlayers(localPlayer);

        {
            std::lock_guard<std::mutex> lock(playersMutex);
            for (const auto& [id, pdata] : players)
            {
                DrawPlayers(pdata);
            }
        }
        SendPlayerDataToServer();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    StopConnection();
    glfwTerminate();
}
void Game::DrawPlayers(const PlayerData& data)
{
    glColor3ub(data.r, data.g, data.b);
    glBegin(GL_QUADS);
    glVertex2f(data.x - data.size / 2, data.y - data.size / 2);
    glVertex2f(data.x + data.size / 2, data.y - data.size / 2);
    glVertex2f(data.x + data.size / 2, data.y + data.size / 2);
    glVertex2f(data.x - data.size / 2, data.y + data.size / 2);
    glEnd();
}
void Game::GetOppDataFromServer()
{
    while (!context.stopped())
    {
        char data[1024];
        asio::ip::udp::endpoint sender_endpoint;
        asio::error_code ec;

        socket->non_blocking();
        if (ec)
        {
            std::cerr << "Error set non_blocking func: " << ec.message() << "\n";
            return;
        }

        size_t len = socket->receive_from(asio::buffer(data), sender_endpoint, 0, ec);
        if (ec == asio::error::would_block)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        else if (ec)
        {
            std::cerr << "Receive error: " << ec.message() << "\n";
            break;
        }

        if (len == sizeof(PlayerData))
        {
            
            PlayerData myData;
            std::memcpy(&myData, data, sizeof(PlayerData));
            localPlayer.id = myData.id;
            localPlayer.r = myData.r;
            localPlayer.g = myData.g;
            localPlayer.b = myData.b;
            
            continue; 
        }
        else if (len >= sizeof(PlayerData))
        {
            std::lock_guard<std::mutex> lock(playersMutex);
            size_t count = len / sizeof(PlayerData);

            const PlayerData* receivedPlayers = reinterpret_cast<const PlayerData*>(data);

            players.clear();

            for (size_t i = 0; i < count; ++i)
            {
                const PlayerData& pdata = receivedPlayers[i];
                if (pdata.id != localPlayer.id)
                {
                    players[pdata.id] = pdata;
                }
            }
        }
    }
}
void Game::SendPlayerDataToServer()
{
    socket->send_to(asio::buffer(&localPlayer, sizeof(PlayerData)), endpoint);
}
void Game::StopConnection()
{
    if (socket->is_open())
    {
        std::error_code ec;
        socket->close(ec);  
    }

    context.stop();

    if (receiveThread.joinable())
        receiveThread.join();  
}