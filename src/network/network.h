#pragma once

namespace Acid
{
    class Network : public boost::noncopyable
    {
        friend class Server;

    private:
        boost::asio::io_service m_Service;
    };
}
