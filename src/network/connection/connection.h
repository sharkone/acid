#pragma once

#include "network/connection/address/address.h"
#include "network/connection/socket/socket.h"

namespace Acid
{
    class Connection : public boost::noncopyable
    {
    public:
        Connection(unsigned int protocolId, float timeout);
        virtual ~Connection();

        bool            Start(const Address& address);
        void            Stop();

        bool            IsDisconnected() const;
        bool            IsConnecting() const;
        bool            IsConnected() const;

        void            Update(float deltaTime);

        bool            SendPacket(const unsigned char* data, std::size_t size);
        std::size_t     ReceivePacket(unsigned char* data, std::size_t size);
    
    protected:
        enum State
        {
            State_Disconnected,
            State_Connecting,
            State_Connected
        };
        
        void            Reset();

        virtual bool    OnStart(const Address& address) = 0;
        virtual void    OnStop()                        = 0;

        virtual bool    OnReceivePacket(const Address& sender) = 0;

        unsigned int    m_ProtocolId;
        float           m_Timeout;

        Socket          m_Socket;
        State           m_State;
        float           m_Timer;
        Address         m_Address;
    };
}
