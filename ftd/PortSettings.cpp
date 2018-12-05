/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifdef _MSC_VER
#include "stdafx.h"
#else
#include "config.h"
#endif

#include "PortSettings.h"
#include "Settings.h"
#include "Values.h"
#include <fstream>

namespace FTD
{
PortSettings::PortSettings( std::istream& stream, bool resolveEnvVars )
throw( ConfigError )
: m_resolveEnvVars( resolveEnvVars )
{
  stream >> *this;
}

PortSettings::PortSettings( const std::string& file, bool resolveEnvVars )
throw( ConfigError )
: m_resolveEnvVars( resolveEnvVars )
{
  std::ifstream fstream( file.c_str() );
  if ( !fstream.is_open() )
    throw ConfigError( ( "File " + file + " not found" ).c_str() );
  fstream >> *this;
}

std::istream& operator>>( std::istream& stream, PortSettings& s )
throw( ConfigError )
{
  Settings settings(s.m_resolveEnvVars);
  stream >> settings;

  Settings::Sections section;

  section = settings.get( "DEFAULT" );
  Dictionary def;
  if ( section.size() )
    def = section[ 0 ];
  s.set( def );

  section = settings.get( "PORT" );
  Settings::Sections::size_type session;
  Dictionary dict;

  for ( session = 0; session < section.size(); ++session )
  {
    dict = section[ session ];
    dict.merge( def );
	std::string protocal = PROTOCAL_TCP;
	std::string host = "127.0.0.1";
	int port = 38888;
	port = dict.getInt(SOCKET_ACCEPT_PORT);
	if (dict.has(SOCKET_CONNECT_HOST))
	{
		host = dict.getString(SOCKET_CONNECT_HOST);
	}
	if (dict.has(SOCKET_PROTOCAL))
	{
		protocal = dict.getString(SOCKET_PROTOCAL);
	}
    s.set( PortID(protocal, host, port), dict );
  }
  return stream;
}

std::ostream& operator<<( std::ostream& stream, const PortSettings& s )
{
  const Dictionary& defaults = s.m_defaults;
  if( defaults.size() )
  {
    stream << "[DEFAULT]" << std::endl;
    Dictionary::iterator i;
    for( i = defaults.begin(); i != defaults.end(); ++i )
      stream << i->first << "=" << i->second << std::endl;
    stream << std::endl;
  }

  std::set<PortID> ports = s.getPorts();
  std::set<PortID>::iterator i;
  for( i = ports.begin(); i != ports.end(); ++i )
  {
    stream << "[PORT]" << std::endl;
    const Dictionary& section = s.get( *i );
    if( !section.size() ) continue;

    Dictionary::iterator i;
    for( i = section.begin(); i != section.end(); ++i )
    {
      if( defaults.has(i->first) && defaults.getString(i->first) == i->second )
        continue;
      stream << i->first << "=" << i->second << std::endl;
    }
    stream << std::endl;
  }

  return stream;
}

const bool PortSettings::has( const PortID& port ) const
{
  return m_settings.find(port) != m_settings.end();
}

const Dictionary& PortSettings::get( const PortID& port ) const
throw( ConfigError )
{
  Dictionaries::const_iterator i;
  i = m_settings.find( port );
  if ( i == m_settings.end() ) throw ConfigError( "Session not found" );
  return i->second;
}

void PortSettings::set( const PortID& port,
                           Dictionary settings )
throw( ConfigError )
{
  //TODO string + int
  if( has(port) )
    throw ConfigError( "Duplicate Port " + port.toStringFrozen() );

  settings.setInt(SOCKET_ACCEPT_PORT, port.getPort());
  settings.merge( m_defaults );
  
  validate( settings );
  m_settings[ port ] = settings;
}

void PortSettings::set( const Dictionary& defaults ) throw( ConfigError ) 
{ 
  m_defaults = defaults;
  Dictionaries::iterator i = m_settings.begin();
  for( i = m_settings.begin(); i != m_settings.end(); ++i )
    i->second.merge( defaults );
}

std::set < PortID > PortSettings::getPorts() const
{
  std::set < PortID > result;
  Dictionaries::const_iterator i;
  for ( i = m_settings.begin(); i != m_settings.end(); ++i )
    result.insert( i->first );
  return result;
}

void PortSettings::validate( const Dictionary& dictionary ) const
throw( ConfigError )
{
  /*
  std::string connectionType = dictionary.getString( CONNECTION_TYPE );
  if( connectionType != "initiator" &&
  connectionType != "acceptor" )
  {
  throw ConfigError( std::string(CONNECTION_TYPE) + " must be 'initiator' or 'acceptor'" );
  }
  */
  
}

}
