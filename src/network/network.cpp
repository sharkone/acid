#include "network/precomp/precomp.h"

#include "network/network.h"

namespace Acid
{
    bool Network::Initialize()
    {
        WSADATA wsaData;
        return (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
    }

    bool Network::Shutdown()
    {
        return (WSACleanup() == 0);
    }
}
