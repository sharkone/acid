#pragma once

#include "network/connection/address/address.h"

namespace Acid
{
    class Network;

    class Client : public boost::noncopyable
    {
    public:
        Client(Network& network, unsigned int protocolId, float timeout);
        ~Client();

        bool                            Start(const Address& address);
        void                            Stop();

        void                            Update(float deltaTime);

        bool                            SendPacket(const unsigned char* data, std::size_t dataSize);
        bool                            SendPacket(const std::vector<unsigned char>& data);

        bool                            ReceivePacket(std::vector<unsigned char>& data);

    private:
        enum State
        {
            State_Disconnected,
            State_Connecting,
            State_Connected
        };

        void                            Reset();

        unsigned int                    m_ProtocolId;
        float                           m_Timeout;

        boost::asio::ip::udp::socket    m_Socket;
        State                           m_State;
        float                           m_Timer;
        Address                         m_Address;
    };
}
