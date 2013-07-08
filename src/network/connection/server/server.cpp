#include "network/precomp/precomp.h"

#include "network/connection/server/server.h"

namespace Acid
{
    Server::Server(unsigned int protocolId, float timeout)
        :   Connection(protocolId, timeout)
    {
    }

    bool Server::OnStart(const Address& address)
    {
        if (m_Socket.Open(address.GetPort(), false) == false)
            return (false);

        Reset();

        ACID_LOG("Server started on port %d.", address.GetPort());
        return (true);
    }

    void Server::OnStop()
    {
        ACID_LOG("Server stopped.");
    }

    bool Server::OnReceivePacket(const Address& sender)
    {
        if (m_State != State_Connected)
        {
            m_State   = State_Connected;
            m_Address = sender;
            ACID_LOG("Connection accepted with %s.", sender.ToString().c_str());
        }

        if (sender != m_Address)
            return (false);

        return (true);
    }
}
