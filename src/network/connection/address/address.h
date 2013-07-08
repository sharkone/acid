#pragma once

namespace Acid
{
    class Address
    {
    public:
        Address();
        Address(unsigned int address, unsigned short port);
        Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);

        unsigned int    GetAddress() const;
        unsigned short  GetPort() const;

        bool            IsValid() const;
        std::string     ToString() const;

        bool operator ==(const Address& other) const;
		bool operator !=(const Address& other) const;
		
    private:
        unsigned int    m_Address;
        unsigned short  m_Port;
    };
}
