#pragma once

namespace Acid
{
    class Address;

    class Socket : public boost::noncopyable
    {
    public:
        Socket();
        ~Socket();

        bool        Open(unsigned short port, bool blocking);
        void        Close();

        bool        IsOpen() const;

        bool        Send(const Address& destination, const void* data, std::size_t size);
        std::size_t Receive(Address& sender, void* data, std::size_t size);

    private:
        SOCKET      m_Socket;
    };
}
