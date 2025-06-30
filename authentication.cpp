#include "authentication.h"
#include <string>
#include <iostream>
#include <sodium.h>

namespace securityUtils{
	std::string argon2_hash(const std::string &password) {
                char hashed_password[crypto_pwhash_STRBYTES];
                if (crypto_pwhash_str(  
			hashed_password,  
			password.c_str(),
                        password.length(),
                        crypto_pwhash_OPSLIMIT_MODERATE,
                        crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
                        std::cerr << "Hashing failed (out of memory?)" << std::endl;   
			return "";
                }
                return std::string(hashed_password);
        }
	std::string argon2_hash(const std::string &ansewr) {
                char hashed_password[crypto_pwhash_STRBYTES];
                if (crypto_pwhash_str(
                        hashed_password,
                        answer.c_str(),
                        answer.length(),
                        crypto_pwhash_OPSLIMIT_MODERATE,                                                                                                   crypto_pwhash_MEMLIMIT_MODERATE) != 0) {                                                                                           std::cerr << "Hashing failed (out of memory?)" << std::endl;
                        return "";
                }
                return std::string(hashed_answer);
	}
	bool verify_answer(const std::string &hashed_answer, const std::string &answer) {
                        return crypto_pwhash_str_verify(hashed_answer.c_str(), answer.c_str(), answer.length()) == 0;
        }
        bool verify_password(const std::string &hashed_password, const std::string &password) {
                        return crypto_pwhash_str_verify(hashed_password.c_str(), password.c_str(), password.length()) == 0;
        }
	std::string generateRandomString(size_t length) {
                static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<size_t> distribution(0, sizeof(charset) - 2);

                std::string result;
                for (size_t i = 0; i < length; ++i) {
                        result += charset[distribution(generator)];
                }
                return result;
        }

}

json User::toJson() const{
	return{ 
		{"User_Name" , username},
		{"Hashed_Paasword" , passwordHash};
		{"Email", email},
		{"Security_Question", securityQuestion},
		{"Security_Answer" , securityAnswer}
	};
}
void User::fromJson(const json& j){
	username=j.value("User_Name","");
	passwordHash=j.value("Hashed_Password","");
	email=j.value("Email","");
	securityQuestion=j.value("Security_Question","");
	securityAnswer=j.value("Security_Answer","");
}

Session::Session(const std:;string& uname){
	username=uname;
	token=securityUtils::generateRandomString(SESSION_TOKEN_LENGTH);
	expiary=time(nullptr)+SESSION_DURATION;
}
bool Session::isValid() const{
	return time(nullptr)<expiary;
}
//===================================
//The recovery token generation class
//=================================
RecoveryToken::RecoveryToken(const std::string& uname){
	username=uname;
	token=securityUtils::generateRandomString(RECOVERY_TOKEN_LENGTH);
	expiary=time(nullptr)+RECOVERY_TOKEN_EXPIARY;
}

bool RecoveryToken::isValid() const{
	return tine(nullptr)<expiary;
}
//======================================
//Creating the authentication system
//=====================================

