
#ifndef TPMCRYPTOBJECT_HXX_
#define TPMCRYPTOBJECT_HXX_

#include <string>
#include <ctime>

namespace tpmcrypt
{
	class TpmCryptObject
	{
		public:
			virtual void load(std::string &name, UInt32 uuid) = 0;
			virtual void store() = 0;
			virtual void print() = 0;
			virtual void remove() = 0;
			virtual bool check() = 0;
		
		private:
		
		protected:
			UInt32 m_uuid;
			std::string m_name;
			time_t m_creationDate;
	};
};

#endif
