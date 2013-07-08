#include "network/precomp/precomp.h"

#include "network/connection/connection.h"

namespace Acid
{
    Connection::Connection(unsigned int protocolId, float timeout)
        :   m_ProtocolId(protocolId)
        ,   m_Timeout(timeout)
    {
        Reset();
    }

    Connection::~Connection()
    {
        if (m_Socket.IsOpen())
            Stop();
    }

    bool Connection::Start(const Address& address)
    {
        ACID_ASSERT(m_Socket.IsOpen() == false);

        if (OnStart(address) == false)
            return (false);
        
        return (true);
    }

    void Connection::Stop()
    {
        ACID_ASSERT(m_Socket.IsOpen() == true);

        Reset();
        m_Socket.Close();
        
        OnStop();
    }

    bool Connection::IsDisconnected() const
    {
        return (m_State == State_Disconnected);
    }

    bool Connection::IsConnecting() const
    {
        return (m_State == State_Connecting);
    }

    bool Connection::IsConnected() const
    {
        return (m_State == State_Connected);
    }
    
    void Connection::Update(float deltaTime)
    {
        ACID_ASSERT(m_Socket.IsOpen() == true);
        
        if (m_State == State_Connecting || m_State == State_Connected)
        {
            m_Timer += deltaTime;
            if (m_Timer > m_Timeout)
            {
                ACID_LOG("Connection time out with %s.", m_Address.ToString().c_str());
                Reset();
            }
        }
    }

    bool Connection::SendPacket(const unsigned char* data, std::size_t size)
    {
        ACID_ASSERT(m_Socket.IsOpen() == true);
        ACID_ASSERT(m_Address.IsValid() == true);

        std::vector<unsigned char> packet;
        packet.resize(size + 4);

        packet[0] = static_cast<unsigned char>((m_ProtocolId >> 24) & 0xFF);
        packet[1] = static_cast<unsigned char>((m_ProtocolId >> 16) & 0xFF);
        packet[2] = static_cast<unsigned char>((m_ProtocolId >>  8) & 0xFF);
        packet[3] = static_cast<unsigned char>((m_ProtocolId >>  0) & 0xFF);
        std::memcpy(&packet[4], data, size);

        return (m_Socket.Send(m_Address, &packet[0], packet.size()));
    }

    std::size_t Connection::ReceivePacket(unsigned char* data, std::size_t size)
    {
        ACID_ASSERT(m_Socket.IsOpen() == true);

        std::vector<unsigned char> packet;
        packet.resize(size + 4);

        Address sender;
        std::size_t bytesReceived = m_Socket.Receive(sender, &packet[0], packet.size());
        if (bytesReceived <= 4)
            return (0);

        if (*reinterpret_cast<unsigned int*>(&packet[0]) != m_ProtocolId)
            return (0);
        
        if (OnReceivePacket(sender) == false)
            return (0);

        m_Timer = 0.0f;
        std::memcpy(data, &packet[4], bytesReceived - 4);

        return (bytesReceived - 4);
    }

    void Connection::Reset()
    {
        m_State   = State_Disconnected;
        m_Timer   = 0.0f;
        m_Address = Address();
    }
}
