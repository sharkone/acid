#pragma once

#include "network/connection/connection.h"

namespace Acid
{
    class Server : public Connection
    {
    public:
        Server(unsigned int protocolId, float timeout);

    protected:
        virtual bool    OnStart(const Address& address);
        virtual void    OnStop();

        virtual bool    OnReceivePacket(const Address& sender);
    };
}
