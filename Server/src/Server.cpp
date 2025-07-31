#include "GameServer.hpp"

int main()
{
    std::unique_ptr<Server> server = std::make_unique<Server>(9000);
    server->Run();
}
