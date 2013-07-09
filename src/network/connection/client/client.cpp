#include "network/precomp/precomp.h"

#include "network/connection/client/client.h"

#include "network/network.h"

namespace Acid
{
    Client::Client(Network& network, unsigned int protocolId, float timeout)
        :   m_ProtocolId(protocolId)
        ,   m_Timeout(timeout)
        ,   m_Socket(network.m_Service)
    {
    }

    Client::~Client()
    {
        if (m_Socket.is_open() == true)
            Stop();
    }

    bool Client::Start(const Address& address)
    {
        ACID_ASSERT(m_Socket.is_open() == false);

        boost::system::error_code errorCode;
        if (m_Socket.open(boost::asio::ip::udp::v4(), errorCode) != 0)
            return (false);

        if (m_Socket.non_blocking(true, errorCode) != 0)
            return (false);

        Reset();
        m_State   = State_Connecting;
        m_Address = address;

        ACID_LOG("Client started connecting to %s.", address.ToString().c_str());
        return (true);
    }

    void Client::Stop()
    {
        ACID_ASSERT(m_Socket.is_open() == true);

        Reset();
        m_Socket.close();

        ACID_LOG("Client stopped.");
    }

    void Client::Update(float deltaTime)
    {
        ACID_ASSERT(m_Socket.is_open() == true);
        
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

    
    bool Client::SendPacket(const unsigned char* data, std::size_t dataSize)
    {
        ACID_ASSERT(m_Socket.is_open() == true);
        ACID_ASSERT(m_Address.IsValid() == true);

        std::vector<unsigned char> packet;
        packet.resize(dataSize + 4);
        
        packet[0] = static_cast<unsigned char>((m_ProtocolId >> 24) & 0xFF);
        packet[1] = static_cast<unsigned char>((m_ProtocolId >> 16) & 0xFF);
        packet[2] = static_cast<unsigned char>((m_ProtocolId >>  8) & 0xFF);
        packet[3] = static_cast<unsigned char>((m_ProtocolId >>  0) & 0xFF);
        
        std::memcpy(&packet[4], &data[0], dataSize);

        boost::asio::ip::udp::endpoint endPoint(boost::asio::ip::address_v4(m_Address.GetAddress()), m_Address.GetPort());
        boost::system::error_code      errorCode;
        
        std::size_t bytesSent = m_Socket.send_to(boost::asio::buffer(packet), endPoint, 0, errorCode);
        if (errorCode != 0)
            return (false);

        if (bytesSent != packet.size())
            return (false);

        return (true);
    }

    bool Client::SendPacket(const std::vector<unsigned char>& data)
    {
        ACID_ASSERT(m_Socket.is_open() == true);
        ACID_ASSERT(m_Address.IsValid() == true);

        std::vector<unsigned char> packet;
        packet.resize(data.size() + 4);
        
        packet[0] = static_cast<unsigned char>((m_ProtocolId >> 24) & 0xFF);
        packet[1] = static_cast<unsigned char>((m_ProtocolId >> 16) & 0xFF);
        packet[2] = static_cast<unsigned char>((m_ProtocolId >>  8) & 0xFF);
        packet[3] = static_cast<unsigned char>((m_ProtocolId >>  0) & 0xFF);
        
        std::copy(data.begin(), data.end(), packet.begin() + 4);

        boost::asio::ip::udp::endpoint endPoint(boost::asio::ip::address_v4(m_Address.GetAddress()), m_Address.GetPort());
        boost::system::error_code      errorCode;
        
        std::size_t bytesSent = m_Socket.send_to(boost::asio::buffer(packet), endPoint, 0, errorCode);
        if (errorCode != 0)
            return (false);

        if (bytesSent != packet.size())
            return (false);

        return (true);
    }

    bool Client::ReceivePacket(std::vector<unsigned char>& data)
    {
        ACID_ASSERT(m_Socket.is_open() == true);

        std::vector<unsigned char> packet;
        packet.resize(data.size() + 4);

        boost::asio::ip::udp::endpoint endPoint;
        boost::system::error_code      errorCode;

        std::size_t bytesReceived = m_Socket.receive_from(boost::asio::buffer(packet), endPoint, 0, errorCode);
        if (errorCode != 0)
            return (false);

        if (bytesReceived <= 4)
            return (false);

        if (*reinterpret_cast<unsigned int*>(&packet[0]) != m_ProtocolId)
            return (false);

        Address sender(endPoint.address().to_v4().to_ulong(), endPoint.port());

        if (sender != m_Address)
            return (false);

        if (m_State == State_Connecting)
        {
            ACID_LOG("Connection established with %s.", sender.ToString().c_str());
            m_State = State_Connected;
        }

        std::size_t dataSize = bytesReceived - 4;

        data.resize(dataSize);
        std::copy(packet.begin() + 4, packet.begin() + 4 + dataSize, data.begin());
        m_Timer = 0.0f;

        return (true);
    }

    void Client::Reset()
    {
        m_State   = State_Disconnected;
        m_Timer   = 0.0f;
        m_Address = Address();
    }
}
