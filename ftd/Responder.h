#ifndef FTD_RESPONDER_H
#define FTD_RESPONDER_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include <string>

namespace FTD
{
  /// Interface implements sending on and disconnecting a transport.
  class Responder
  {
  public:
	  typedef std::shared_ptr<Responder> SPtr;
	  typedef std::weak_ptr<Responder> WPtr;
    virtual ~Responder() {}
    virtual bool send( const std::string& ) = 0;
    virtual void disconnect() = 0;
  };
}

#endif
