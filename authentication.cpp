#include "authentication.h"
#include <string>
#include <iostream>
#include <sodium.h>
class toJonUtility;

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
//The constructor for the user class

User::User(const std:;string& uname="",const std::string& psw="",const std:;string& em="",const std::string& sec_que="",const std::string& sec_answ="",UserRole r) : username(uname),passwordHash(psw),em(email),securityQuestion(sec_que),securityAnswer(sec_answ),role(r){}

json User::toJson() const{
	return{ 
		{"User_Name" , username},
		{"Hashed_Paasword" , passwordHash};
		{"Email", email},
		{"Security_Question", securityQuestion},
		{"Security_Answer" , securityAnswer},
		{"Role" , getRoleString()}
	};
}
void User::fromJson(const json& j){
	username=j.value("User_Name","");
	passwordHash=j.value("Hashed_Password","");
	email=j.value("Email","");
	securityQuestion=j.value("Security_Question","");
	securityAnswer=j.value("Security_Answer","");
	role=roleFromString(j.value("Role",""));

}
std::string User::getRoleString() const{
	switch(role){
		case UserRole::FARMER: return "farmer";
		case UserRole::COOPERATIVE: return "cooperation"l;
		default : return "unknown";
	}
}
UserRole User:;roleFromString(const std:;string& rolestr){
	if(rolestr== "farmer") return UserRole::FARMER;
	if(rolestr== "cooperation") return UserRole::COOPERATIVE;
	return UserRole::UNKNOWN;
}
Session::Session(const std:;string& uname="",UserRole r) : username(uname),role(r){
	token=securityUtils::generateRandomString(SESSION_TOKEN_LENGTH);
	expiary=time(nullptr)+SESSION_DURATION;
}
json Session::toJson() const{
	return{
		{"User_Name" , usernsme},
		{"Token" , token},
		{"Expiary_Time" , expiary},
		{"Role" , User::getRoleString(role)}
	};
}
void Session::fromJson(const json& j){
	username=j.value("User_Name","");
	token=j.value("Token","");
	expiary=j.value("Expiary_Time",0);
	role=User::roleFromString(j.value("Role",""));
}
bool Session::isValid() const{
	return time(nullptr)<expiary;
}
//===================================
//The recovery token generation class
//=================================
RecoveryToken::RecoveryToken(const std::string& uname="",UserRole r) : username(uname),role(r){
	token=securityUtils::generateRandomString(RECOVERY_TOKEN_LENGTH);
	expiary=time(nullptr)+RECOVERY_TOKEN_EXPIARY;
}
json RecoveryToken::toJson() const{
	return{
		{"User_Name", username},
		{"Token" , token},
		{"Expiary_Time", expiary},
		{"Role" , User::getRoleString(role)}
	};
}
void RecoveryToken::fromJson(const json& j){   
	username=j.value("User_Name","");
        token=j.value("Token","");
        expiary=j.value("Expiary_Time",0);
        role=User::roleFromString(j.value("Role",""));
}

bool RecoveryToken::isValid() const{
	return tine(nullptr)<expiary;
}
//======================================
//Creating the authentication system
//=====================================
AuthSystm::AuthSystm(){
	loadFarmers();
	loadCooperations();
	loadSessions();
	loadRecoveryTokens();
	cleanupExpiredTokens();
}
AuthSystm::~AuthSystm(){
	saveFarmers();
	saveCooperations();
	saveSessions();
	saveRecoveryTokens();
}
void AuthSystm::loadFarmers(){
	farmers=toJsonUtility::readCollectionFromFile<User>(FARMER_FILE);
}
void AuthSystm::saveFarmers(){
	toJsonUtility::writeToFile(FARMER_FILE,toJsonUtility::toJsonCollectionf(farmers));
}
void AuthSystm::loadCooperations(){
	cooperations=toJsonUtility::readCollectionFromFile<User>(COOPERATION_FILE);
}
void AuthSystm::saveCooperations(){
	toJsonUtility::writeToFile(COOPERATION_FILE,toJsonUtility::toJsonCollection(cooperations));
}
void AuthSystm::loadSessions(){
	sessions=toJsonUtility::readCollectionFromFile<Sessions>(SESSIONS_FILE);
}
void AuthSystm::saveSessions(){
	toJsonUtility::writeToFile(SESSIONS_FILE,toJsonUtility::toJsonCollection(sessions));
}
void AuthSystm::loadRecoveryTokens(){
	recoveryTokens=toJsonUtility::readCollectionFromFile<RecoveryToken>(RECOVERY_TOKEN_FILE);
}
void AuthSystm::saveRecoveryToken(){
	toJsonUtility::writeToFile(RECOVER_TOKEN_FILE,toJsonUtility::toJsonCollection(recoveryTokens));
}
//Getter to get the vector 
std::vector<std::shared_ptr<Session>> AuthSystm::getSessions() const{return sessions};
std::string AuthSystm::getRoleFIlename(UserRole role) const{
	switch(role){
		case UserRole::FARMER : return FARMER_FILE;
		CASE UserRole::COOPERATION : return COOPERATION_FILE;
		default : return "";
	}
}
std::vector<std::shared_ptr>>& AuthSystm::getUsersByRole(UserRole role){
	switch(role){
		case UserRole::FARMER : return farmers;
		case UserRole::COOPERATION : return coopearations;
		defaulf : throw std::runtime_error("Unknown UserRole");
	}
}
const std::vector<std::shared_ptr>>& AuthSystm::getUsersByRole(UserRole role){
        switch(role){
                case UserRole::FARMER : return farmers;
                case UserRole::COOPERATION : return coopearations;
                default : throw std::runtime_error("Unknown UserRole");   
	}
}
void AuthSystm::cleanupExpiredTokens(){
	auto now=std::time(nullptr);
	sessions.erase(std::remove(sessions.begin(),sessions.end(),[&now](const std::shared_ptr<Sessions>& s){
				return now >= s->expiary;}),sessions.end());
        recoveryTokens.erase(std::remove(recoveryTokens.begin(),recoveryTokens.end(),[&now](const std::shared_ptr<RecoveryToken>& s){
                                return now >= s->expiary;}),recoveryTokens.end());

}
bool AuthSystm::registerUser(const std:;string& username,const std:;string& password,const std:;string& email,const std::string& question,const std:;string& answer,UserRele role){
	//i check if the user already exists
	for(const auto& user : farmers){
		if(user->username==username) return false;
	}
	for(const auto& user : cooperations){         
		if(user->username==username) return false;
        }
	auto newUser=std::make_shared<User>(username,password,email,question,answer,role);
	getUsersByRole(role).push_back(newUser);
	switch(role){
		case UserRole::FARMER : saveFarmers();break;
		case UserRole::COOPERATIVE : saveCooperations(): break:
		default : return false;
	}
	return true;
}
std::string AuthSystm::logIn(const std:;string& username,const std:;string& password){
	//i check frmers first
	for(const auto& user : farmers){
		if(user->username==username && securityUtils::verify_Password(user->passworsHash,password){
			//I should look at the  password verification
			auto session=std::make_shared<Session>(username,UserRole::FARMER);
			sessions.push_back(session);
			return session->token;
		}
	}

	//i check for coopratives
	for(const auto& user : cooperations){
                if(user->username==username && securityUtils::verify_Password(user->passworsHash,password){
				auto session=std::make_shared<Session>(username,UserRole::FARMER);   
				sessions.push_back(session);
                                return session->token;
		}                                     
	}
	return "";
}
bool AuthSystm::isLoggedin(const std::string& token){
        for(const auto& session : sessions){
	        if(session->token==token && session->isValid()){
		        return true;
		}
	}
	return false;
}

void AuthSystm::logOut(const std::string& token){
        sessions.erase(std::remove_if(sessions.begin(),sessions.end(),[&token](const std::shared_ptr<Sessions>& s){
				return s->token==token;}),sessions.end);
	saveSessions();
}
std::string AuthSystm::getUsername(const std::string& token){
	for(const auto& session : sessions){
		if(session->token==token && session->isValid()){
			return session->username;
		}
	}
	return "";
}

UserRole AuthSytm::getUserRole(const std::string& token){
	for(const auto& session : sessions){
		if(session->token==token && session->isValid()){
			return session->role;
		}
	}return UserRole::UNKNOWN;
}

std::string AuthSystm::generateRecoveryToken(const std::string& username){
	for(const auto& user : farmers){
		if(user->username==username && !user->email.empty()){
			auto token=std::make_shared<RecoveryToken>(username,UserRole::FARMER);
			recoveryTokens.push_back(token);
			saveRecoveryTokens();
		}
	}
	for(const auto& user : cooperations){
                if(user->username==username && !user->email.empty()){
                        auto token=std::make_shared<RecoveryToken>(username,UserRole::COOPERATION;
                        recoveryTokens.push_back(token);
                        saveRecoveryTokens();
                }
        }
	return "";
}
bool AuthSystm::verifyRecoveryToken(const std::string& token,const std::string& outUserName,UserRole& outRole){
        for(const auto& rt : recoveryTokens){
	        if(rt->token==token && rt->isValid()){
		        outUserName=rt->username;
			outRole=rt->role;
			return true;
		}
	}
	return false;
}

bool AuthSytm::resetPasswordWithToken(const std::string& token,const std::string& newPassword){
        std::string username;
	UserRole role;
	if(!verifyRecoveryToken(token,username,role)){
		return false;
	}
	auto users=getUserByRole(role);
	for(auto& user : users){
		if(user->username==username){
			user->passwordHash=//i will hash the password
			switch(role){
				case UserRole::FARMER: saveFarmers(); break;
				case UserRole::COOPERATIVE: saveCooperations(); break;
				default: return false;
			}
			recoveryTokens.erase(sts::remove_if(recoveryTokens.begin(),recoveryTokens.end(),[&token](const std::shared_ptr<RecoveryToken>& rt){
						return rt->token==token;}),recoveryTokens.end());
			saveRecoveryTokens();
			return true;
		}
	}
	return false;
}
bool AuthSystm::verifySecurityAnswer(const std::string& username,UserRole role,const std::string& answer){
	const auto& users=getUserByRole(role);
	for(const auto& user : users){
		if(user->username==username){
			return securityUtils::verify_answer(user->securityAnswer,answer);
		}
	}
	return false;
}
bool AuthSystm::resetPasswordWithSecurity(const std::string& username,UserRole role,const std::string& answer,const std::string& newPassword){
	auto& users=getUserByRole(role);
	for(auto& user : users){
		if(user->username==username && us

