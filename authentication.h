#ifdef AUTH_H
#define AUTH_H
#include <vector>
#include <string>
#include <ctime>

inline constexpr SESSION_TOKEN_LENGTH=32;
inline constexpr RECOVERY_TOKEN_LENGTH=32;
inline constexpr SESSION_DURATION=1800;
inline constexpr RECOVERY_TOKENEXPIARY=3600;

namespace securityUtils{
	std::string argon2_hash(const std::string& password);
	std::string argon2_hash(const std::string& answer);
	bool verify_answer(const std::string& hashed_answer,const std::string& answer)
	bool verify_password(const std::string& hashed_password, const std::string& password);
	std::string generateRandomString(size_t length);
}

struct User{
	std::string username;
	std::strinh passwordHash;
	std::string email;
	std::string securityQuestion;
	std::string securityAnswer;
	json toJson() const;
	void fromJson(const json& j);
};

class Session{
	public:
		std::string username;
		std::string token;
		time_t expiary;
		Session(const string& uname);
		bool isValid() const;
};
class RecoveryToken{
	public:
		std::string username;
		std::string token;
		time_t expiary;

		RecoveryToken(const std::sting& uname);
		bool isValid() const;
};

class AuthSystm{




