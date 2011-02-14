// Copyright (C) 2008, 2009 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

/**
 * @file        TpmCryptSubject.cxx
 * @brief       Implementation of TpmCrypts subject class.
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#include <iostream>

#include <TpmCryptPlatform.hxx>
#include <TpmCryptTPM.hxx>
#include <TpmCryptStorage.hxx>


using namespace std;
using namespace tpmcrypt;


TpmCryptPlatform::TpmCryptPlatform()
{
}

TpmCryptPlatform::TpmCryptPlatform(std::string &name)
	: m_name(name)
{
	// load configuration represented by given name
}

TpmCryptPlatform::TpmCryptPlatform(UInt32 uuid)
	: m_uuid(uuid)
{
	// load configuration represented by given UUID
}

TpmCryptPlatform::TpmCryptPlatform(std::string &name, std::vector<UInt8> pcrs)
	: m_name(name)
	, m_pcrs(pcrs)
{
	
}

TpmCryptPlatform::TpmCryptPlatform(cpnst TpmCryptPlatform &copy)
	: m_name(copy.m_name)
	, m_uuid(copy.m_uuid)
	, m_creationDate(copy.m_creationDate)
	, m_pcrs(copy.m_pcrs)
	, m_authSecret(copy.m_authSecret)
{
}

void TpmCryptPlatform::load(std::string &name, UInt32 uuid)
{
}

void TpmCryptPlatform::store()
{
}

void TpmCryptPlatform::print()
{
	// convert time_t to string
	char buffer[20];
	strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", m_creationDate);
	
	// print platform configuration
	cout << "PLATFORM: " << m_name << "(" << m_uuid << ")" << endl;
	cout << "\tcreation date: " << buffer << endl;
	cout << "\tauthentication secret: " << m_authSecret << endl;
	cout << "\tPCRs:";
	vector<UInt8>::iterator it = m_pcrs.begin();
	for(; it != m_pcrs.end(); it++)
	{
		cout << " " << (*it);
	}
	cout << endl;
}

void TpmCryptPlatform::unstore()
{
}

bool TpmCryptPlatform::check()
{
}
