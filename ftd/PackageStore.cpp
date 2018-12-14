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

#include "PackageStore.h"

namespace FTD
{
	PackageStore* PackageStoreFactory::create(const SessionID&)
	{
		return new MemoryStore();
	}

	void PackageStoreFactory::destroy(PackageStore* pStore)
	{
		delete pStore;
	}




	bool PackageStore::set(int msgSeqNum, const std::string& msg)
		throw(IOException)
	{
		return true;
	}

	void PackageStore::get(int begin, int end,
		std::vector < std::string > & messages) const
		throw(IOException)
	{
	}

	PackageStore* MemoryStoreFactory::create(const SessionID&)
	{
		return new MemoryStore();
	}

	void MemoryStoreFactory::destroy(PackageStore* pStore)
	{
		delete pStore;
	}

	bool MemoryStore::set(int msgSeqNum, const std::string& msg)
		throw(IOException)
	{
		return true;
	}

	void MemoryStore::get(int begin, int end,
		std::vector < std::string > & messages) const
		throw(IOException)
	{
	}
} //namespace FIX
