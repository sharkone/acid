#include "network/precomp/precomp.h"

#include "network/connection/client/client.h"

namespace Acid
{
    Client::Client(unsigned int protocolId, float timeout)
        :   Connection(protocolId, timeout)
    {
    }

    bool Client::OnStart(const Address& address)
    {
        if (m_Socket.Open(0, false) == false)
            return (false);

        Reset();

        m_State   = State_Connecting;
        m_Address = address;
        ACID_LOG("Client started connecting to %s.", address.ToString().c_str());
        return (true);
    }

    void Client::OnStop()
    {
        ACID_LOG("Client stopped.");
    }

    bool Client::OnReceivePacket(const Address& sender)
    {
        if (sender != m_Address)
            return (false);

        if (m_State == State_Connecting)
        {
            ACID_LOG("Connection established with %s.", sender.ToString().c_str());
            m_State = State_Connected;
        }

        return (true);
    }
}
