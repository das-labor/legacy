
#include <TpmCryptDAO.hxx>

TpmCryptDAO::TpmCryptDAO()
{
}

TpmCryptDAO::~TpmCryptDAO()
{
}

static TpmCryptDAO& TpmCryptDAO::instance()
{
}

TpmCryptPlatform* TpmCryptDAO::findByUUID(UInt32 uuid)
{
}

TpmCryptPlatform* TpmCryptDAO::findByName(std::string &name)
{
}

void TpmCryptDAO::persist(TpmCryptPlatform &platform)
{
}

void TpmCryptDAO::remove(TpmCryptPlatform &platform)
{
}

TpmCryptVolume* TpmCryptDAO::findByUUID(UInt32 uuid)
{
}

TpmCryptVolume* TpmCryptDAO::findByName(std::string &name)
{
}

void TpmCryptDAO::persist(TpmCryptVolume &volume)
{
}

void TpmCryptDAO::remove(TpmCryptVolume &volume)
{
}

TpmCryptUser* TpmCryptDAO::findByUUID(UInt32 uuid)
{
}

TpmCryptUser* TpmCryptDAO::findByName(std::srting &name)
{
}

void TpmCryptDAO::persist(TpmCryptUser &user)
{
}

void TpmCryptDAO::remove(TpmCryptUser &user)
{
}

TpmCryptToken* TpmCryptDAO::findByUUID(UInt32 uuid)
{
}

TpmCryptToken* TpmCryptDAO::findByName(std::string &name)
{
}

void TpmCryptDAO::persist(TpmCryptToken &token)
{
}

void TpmCryptDAO::remove(TpmCryptToken &token)
{
}
