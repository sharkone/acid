#pragma once

#include "network/connection/connection.h"

namespace Acid
{
    class Client : public Connection
    {
    public:
        Client(unsigned int protocolId, float timeout);

    protected:
        virtual bool    OnStart(const Address& address);
        virtual void    OnStop();

        virtual bool    OnReceivePacket(const Address& sender);
    };
}
