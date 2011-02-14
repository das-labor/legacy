
#include <TpmCryptPlatform.hxx>
#include <TpmCryptVolume.hxx>
#include <TpmCryptUser.hxx>
#include <TpmCryptSSS.hxx>


namespace tpmcrypt
{
	class TpmCryptDAO
	{
		public:
			static TpmCryptDAO& instance();
			
			// platform 
			TpmCryptPlatform* findByUUID(UInt32 uuid);
			TpmCryptPlatform* findByName(std::string &name);
			void persist(TpmCryptPlatform &platform);
			void remove(TpmCryptPlatform &platform);
			
			// volume
			TpmCryptVolume* findByUUID(UInt32 uuid);
			TpmCryptVolume* findByName(std::string &name);
			void persist(TpmCryptVolume &volume);
			void remove(TpmCryptVolume &volume);
			
			// user
			TpmCryptUser* findByUUID(UInt32 uuid);
			TpmCryptUser* findByName(std::srting &name);
			void persist(TpmCryptUser &user);
			void remove(TpmCryptUser &user);
			
		
		private:
			TpmCryptDAO();
			~TpmCryptDAO();
			
			static TpmCryptDAO m_instance;
	};
};
