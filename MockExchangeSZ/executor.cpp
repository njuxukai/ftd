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
#pragma warning( disable : 4503 4355 4786 )
#else
#include "config.h"
#endif

#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>

void wait()
{
  std::cout << "Type Ctrl-C to quit" << std::endl;
  while(true)
  {
    FIX::process_sleep(1);
  }
}

int main( int argc, char** argv )
{

  std::string stepSpecFile = "step/executor.cfg";
  

  FIX::Acceptor * acceptor = 0;
  try
  {
    FIX::SessionSettings settings(stepSpecFile);

    Application application("mock_exchange_sz.cfg");
    //FIX::FileStoreFactory storeFactory( settings );
	FIX::MemoryStoreFactory storeFactory;
    FIX::ScreenLogFactory logFactory( settings );

    acceptor = new FIX::SocketAcceptor ( application, storeFactory, settings, logFactory );
    acceptor->start();
    wait();
    acceptor->stop();
    delete acceptor;
    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what() << std::endl;
    delete acceptor;
    return 1;
  }
}
