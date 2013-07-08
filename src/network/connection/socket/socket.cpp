#include "network/precomp/precomp.h"

#include "network/connection/socket/socket.h"

#include "network/connection/address/address.h"

namespace Acid
{
    Socket::Socket()
        :   m_Socket(INVALID_SOCKET)
    {
    }

    Socket::~Socket()
    {
        Close();
    }

    bool Socket::Open(unsigned short port, bool blocking)
    {
        ACID_ASSERT(IsOpen() == false);

        m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_Socket == INVALID_SOCKET)
            return (false);

        sockaddr_in address;
        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port        = htons(port);
		
        if (bind(m_Socket, (const sockaddr*)&address, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            Close();
            return (false);
        }

        if (blocking == false)
        {
            DWORD nonBlocking = 1;
            if (ioctlsocket(m_Socket, FIONBIO, &nonBlocking) == SOCKET_ERROR)
            {
                Close();
                return (false);
            }
        }

        return (true);
    }

    void Socket::Close()
    {
        if (m_Socket != INVALID_SOCKET)
        {
            bool result = (closesocket(m_Socket) != SOCKET_ERROR);
            m_Socket    = INVALID_SOCKET;
        }
    }

    bool Socket::IsOpen() const
    {
        return (m_Socket != INVALID_SOCKET);
    }

    bool Socket::Send(const Address& destination, const void* data, std::size_t size)
    {
        ACID_ASSERT(IsOpen() == true);
        ACID_ASSERT(destination.IsValid() == true);
        ACID_ASSERT(data != nullptr);
        ACID_ASSERT(size > 0);

        sockaddr_in address;
        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = htonl(destination.GetAddress());
        address.sin_port        = htons(destination.GetPort());

        if (sendto(m_Socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in)) != size)
            return (false);

        return (true);
    }

    std::size_t Socket::Receive(Address& sender, void* data, std::size_t size)
    {
        ACID_ASSERT(IsOpen() == true);
        ACID_ASSERT(data != nullptr);
        ACID_ASSERT(size > 0);
    
        sockaddr_in from;
        int         fromLength = sizeof(from);

        int result = recvfrom(m_Socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength);
        if (result == SOCKET_ERROR)
            return (0);

        sender = Address(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));

        return (result);
    }
}
