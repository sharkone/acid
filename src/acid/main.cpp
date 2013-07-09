#include "acid/precomp/precomp.h"

#include "network/network.h"
#include "network/connection/client/client.h"
#include "network/connection/server/server.h"

#define PROTOCOL_ID 0x42424242
#define TIMEOUT     10.0f
#define PORT        4242

int main(int argc, char** argv)
{
    Acid::Network network;

    if (std::string(argv[1]) == "-server")
    {
        Acid::Server server(network, PROTOCOL_ID, TIMEOUT);
        server.Start(PORT);

        std::size_t msg_cnt = 0;

        while (true)
        {
            server.Update(0.033f);

            std::vector<unsigned char> packet(512);
            if (server.ReceivePacket(packet) != 0)
            {
                std::cout << "Server received: " << std::string(packet.begin(), packet.end()) << std::endl;

                std::string msg = std::string("Server world: ") + boost::lexical_cast<std::string>(msg_cnt++);
                server.SendPacket((const unsigned char*)msg.c_str(), msg.size());
            }

            Sleep(33);
        }
    }
    else if (std::string(argv[1]) == "-client")
    {
        Acid::Client client(network, PROTOCOL_ID, TIMEOUT);
        client.Start(Acid::Address(127, 0, 0, 1, PORT));

        std::size_t msg_cnt = 0;
        std::string msg     = std::string("Client world: ") + boost::lexical_cast<std::string>(msg_cnt++);

        client.SendPacket((const unsigned char*)msg.c_str(), msg.size());

        while (true)
        {
            client.Update(0.033f);

            std::vector<unsigned char> packet(512);
            if (client.ReceivePacket(packet) != 0)
            {
                std::cout << "Client received: " << std::string(packet.begin(), packet.end()) << std::endl;

                msg = std::string("Client world: ") + boost::lexical_cast<std::string>(msg_cnt++);
                client.SendPacket((const unsigned char*)msg.c_str(), msg.size());
            }

            Sleep(33);
        }
    }

    return (0);
}
