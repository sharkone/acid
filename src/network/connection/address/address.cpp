#include "network/precomp/precomp.h"

#include "network/connection/address/address.h"

namespace Acid
{
    Address::Address()
        :   m_Address(0)
        ,   m_Port(0)
    {
    }

    Address::Address(unsigned int address, unsigned short port)
        :   m_Address(address)
        ,   m_Port(port)
    {
    }

    Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
        :   m_Address((a << 24) | (b << 16) | (c << 8) | d)
        ,   m_Port(port)
    {
    }

    unsigned int Address::GetAddress() const
    {
        return (m_Address);
    }

    unsigned short Address::GetPort() const
    {
        return (m_Port);
    }

    bool Address::IsValid() const
    {
        return (m_Address != 0 && m_Port != 0);
    }

    std::string Address::ToString() const
    {
        return (boost::lexical_cast<std::string>(m_Address >> 24 & 0xFF) + "." +
                boost::lexical_cast<std::string>(m_Address >> 16 & 0xFF) + "." +
                boost::lexical_cast<std::string>(m_Address >> 8  & 0xFF) + "." +
                boost::lexical_cast<std::string>(m_Address >> 0  & 0xFF) + ":" +
                boost::lexical_cast<std::string>(m_Port));
    }

    bool Address::operator ==(const Address& other) const
    {
        return (m_Address == other.m_Address && m_Port == other.m_Port);
    }
    
    bool Address::operator !=(const Address& other) const
    {
        return (m_Address != other.m_Address || m_Port != other.m_Port);
    }
}
