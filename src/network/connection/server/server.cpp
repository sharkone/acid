#include "network/precomp/precomp.h"

#include "network/connection/server/server.h"

#include "network/network.h"

namespace Acid
{
    Server::Server(Network& network, unsigned int protocolId, float timeout)
        :   m_ProtocolId(protocolId)
        ,   m_Timeout(timeout)
        ,   m_Socket(network.m_Service)
    {
        Reset();
    }

    Server::~Server()
    {
        if (m_Socket.is_open() == true)
            Stop();
    }

    bool Server::Start(unsigned short port)
    {
        ACID_ASSERT(m_Socket.is_open() == false);
        
        boost::system::error_code errorCode;
        if (m_Socket.open(boost::asio::ip::udp::v4(), errorCode) != 0)
            return (false);

        if (m_Socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port), errorCode) != 0)
            return (false);

        if (m_Socket.non_blocking(true, errorCode) != 0)
            return (false);

        Reset();
        m_State = State_Running;

        ACID_LOG("Server started on port %d.", port);
        return (true);
    }

    void Server::Stop()
    {
        ACID_ASSERT(m_Socket.is_open() == true);

        Reset();
        m_Socket.close();
        
        ACID_LOG("Server stopped.");
    }

    void Server::Update(float deltaTime)
    {
        ACID_ASSERT(m_Socket.is_open() == true);
        
        if (m_State == State_Connected)
        {
            m_Timer += deltaTime;
            if (m_Timer > m_Timeout)
            {
                ACID_LOG("Connection time out with %s.", m_Address.ToString().c_str());
                Reset();
            }
        }
    }

    bool Server::SendPacket(const unsigned char* data, std::size_t dataSize)
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

    bool Server::SendPacket(const std::vector<unsigned char>& data)
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

    bool Server::ReceivePacket(std::vector<unsigned char>& data)
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

        if (m_State != State_Connected)
        {
            m_State   = State_Connected;
            m_Address = sender;
            ACID_LOG("Connection accepted with %s.", sender.ToString().c_str());
        }

        if (sender != m_Address)
            return (false);

        std::size_t dataSize = bytesReceived - 4;

        data.resize(dataSize);
        std::copy(packet.begin() + 4, packet.begin() + 4 + dataSize, data.begin());
        m_Timer = 0.0f;

        return (true);
    }

    void Server::Reset()
    {
        m_State   = State_Stopped;
        m_Timer   = 0.0f;
        m_Address = Address();
    }
}
