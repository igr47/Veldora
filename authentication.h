#ifdef AUTH_H
#define AUTH_H
#include <vector>
#include <string>
#include <ctime>

inline constexpr int SESSION_TOKEN_LENGTH=32;
inline constexpr int RECOVERY_TOKEN_LENGTH=32;
inline constexpr int SESSION_DURATION=1800;
inline constexpr int RECOVERY_TOKENEXPIARY=3600;
inline const std::string FARMER_FILE="faemers.json";
inline const std::sting COOPERATIONS_FILE="cooperations.json";
inline const std::string SESSIONS_FILE="sessions.json";
inline const std::string RECOVERY_TOKENS_FILE="recovery.json";
enum class UserRole{
	FARMER,
	COOPERATION,
	UNKNOWN
}

namespace securityUtils{
	std::string argon2_hash(const std::string& password);
	std::string argon2_hash(const std::string& answer);
	bool verify_answer(const std::string& hashed_answer,const std::string& answer)
	bool verify_password(const std::string& hashed_password, const std::string& password);
	std::string generateRandomString(size_t length);
}

class  User{
	public:
	        std::string username;
	        std::strinh passwordHash;
	        std::string email;
	        std::string securityQuestion;
	        std::string securityAnswer;
	        UserRole role;
		User(const std::string& uname="",const std::string& psw="",const std::string& em="",const std::string& sec_que="",const std::string& sec_answ="",UserRole r=UserRole::UNKNOWN);
	        json toJson() const;
	        void fromJson(const json& j);
		std::string getRoleString() const;
		static UserRole roleFromString(const std::string& rolestr);
};

class Session{
	public:
		std::string username;
		std::string token;
		time_t expiary;
		UserRole role;
		Session(const string& uname="",UserRole r=UserRolE::UNKNOWN);
		bool isValid() const;
		json toJson() const;
		void fromJson(const json& j);
};
class RecoveryToken{
	public:
		std::string username;
		std::string token;
		time_t expiary;
		UserRole role;
		RecoveryToken(const std::sting& uname="",UserRole r=UserRole::UNKNOWN);
		bool isValid() const;
		json toJson() const;
		void fromJson(const json& j);
};

class AuthSystm{
	private:
		std::vector<std:;shared_ptr<User>> farmers;
		std::vector<std::shared_ptr<User>> cooperations;
		std::vector<std::shared_ptr<Session>> sessions;
		std::vector<std:;shared_ptr<TecoveryToken>> recoveryTokens;

		void loadFarmers();
		void saveFarmers();
		void loadCooperations();
		void saveCooperations();
		void loadSessions();
		void saveSessions();
		void loadRecoveryToken();
		void saveRecoveryToken();
		void cleanupExpiredTokens();
		std::string getRoleFilename(UserRole role) const;
		std::vector<std::shared_ptr<User>>& getUserByRole(UserRole role) const;
		const std:;vector<std::shared_ptr<User>>& getUserByRole(UserRole role) const;
	public:
		AuthSystm();
		~AuthSystm();
		bool registerUser(const std:;string& username,const std:;string& password,const std:;string& email,const std::string& question,const std:;string& answer,UserRele role);
		std::string logIn(const std:;string& username,const std:;string& password);
		bool isLoggedin(const std::string& token);
		void logOut(const std::string& tolen);
		std::string getUsername(const std::string& token);
		UserRole getUserRole(const std::string& token);
		std::string generateRecoveryToken(const std::string& username);
		bool verifyRecoveryToken(const std::string& token,const std::string& outUserName,UserRole& outRole);
		bool resetPasswordWithToken(const std::string& token,const std::string& newPassword);
		bool verifySecurityAnswer(const std::string& username,UserRole role,const std::string& answer);
		bool resetPasswordWithSecurity(const std::string& username,UserRole role,const std::string& answer,const std::string& newPassword);

};
#endif



