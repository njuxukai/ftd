#ifndef FTD_PORT_H
#define FTD_PORT_H

#include <string>
#include "FtdConvertors.h"
#define PROTOCAL_TCP "tcp"
#define PROTOCAL_UDP "udp"

namespace FTD
{
class PortID
{
public:
	PortID()
	{
		toString(m_frozenString);
	}

	PortID(const std::string& protocal,
		const std::string& address,
		int  port)
	: m_protocal(protocal),
	  m_address(address),
	  m_port(port)
	{
		toString(m_frozenString);
	}

	const std::string& getProtocal() const
	{ 
		return m_protocal;
	}

	const std::string& getAddress() const
	{
		return m_address;
	}

	const int getPort() const
	{
		return m_port;
	}

	std::string toString() const
	{
		return m_frozenString;
	}

	const std::string& toStringFrozen() const
	{
		return m_frozenString;
	}

	void fromString(const std::string& str)
	{
		std::string::size_type first = str.find_first_of(':');
		std::string::size_type second = str.find_last_of(':');
		m_protocal = str.substr(0, first);
		m_port = IntConvertor::convert(str.substr(second + 1));
		std::string::size_type addressBegin = first + 1;
		if (addressBegin < str.size())
		{
			while(str[addressBegin] == '\\' 
				|| str[addressBegin] == '/')
			{
				addressBegin++;
			}
			m_address = str.substr(addressBegin, second - addressBegin);
		}
		toString(m_frozenString);
	}

	void toString(std::string& target)
	{
		target = m_protocal + ":\\" + m_address + ":" + 
			IntConvertor::convert(m_port);
	}

	friend bool operator<(const PortID&, const PortID&);
	friend bool operator==(const PortID&, const PortID&);
	friend bool operator!=(const PortID&, const PortID&);
	friend std::ostream& operator<<(std::ostream&, const PortID&);
	friend std::ostream& operator>>(std::ostream&, const PortID&);

	std::string m_frozenString;
	std::string m_protocal;
	std::string  m_address;
	int m_port;
};

inline bool operator<(const PortID& lhs, const PortID& rhs)
{
	return lhs.toStringFrozen() < rhs.toStringFrozen();
}

inline bool operator==(const PortID& lhs, const PortID& rhs)
{
	return lhs.toStringFrozen() == rhs.toStringFrozen();
}

inline bool operator!=(const PortID& lhs, const PortID& rhs)
{
	return !(lhs == rhs);
}

inline std::ostream& operator<<
(std::ostream& stream, const PortID& portID)
{
	stream << portID.toStringFrozen();
	return stream;
}

inline std::istream& operator>>
(std::istream& stream, PortID& portID)
{
	std::string str;
	stream >> str;
	portID.fromString(str);
	return stream;
}
}

#endif