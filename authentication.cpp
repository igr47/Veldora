#include "authentication.h"
#include <string>
#include <iostream>
#include <sodium.h>
#include "fileSaver.h"
#include <algorithm>
#include <random>

// Initialize libsodium (call this at program startup)
__attribute__((constructor))
static void init_sodium() {
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium" << std::endl;
        exit(EXIT_FAILURE);
    }
}

namespace securityUtils {
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

    std::string argon2_hash(const std::string &answer) {
        char hashed_answer[crypto_pwhash_STRBYTES];
        if (crypto_pwhash_str(
            hashed_answer,
            answer.c_str(),
            answer.length(),
            crypto_pwhash_OPSLIMIT_MODERATE,
            crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
            std::cerr << "Hashing failed (out of memory?)" << std::endl;
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
        result.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            result += charset[distribution(generator)];
        }
        return result;
    }
}

// User class implementation
User::User(const std::string& uname, const std::string& psw, const std::string& em,
           const std::string& sec_que, const std::string& sec_answ, UserRole r) :
    username(uname),
    passwordHash(securityUtils::argon2_hash(psw)),
    email(em),
    securityQuestion(sec_que),
    securityAnswer(securityUtils::argon2_hash(sec_answ)),
    role(r) {}

nlohmann::json User::toJson() const {
    return {
        {"User_Name", username},
        {"Hashed_Password", passwordHash},
        {"Email", email},
        {"Security_Question", securityQuestion},
        {"Security_Answer", securityAnswer},
        {"Role", getRoleString()}
    };
}

void User::fromJson(const nlohmann::json& j) {
    username = j.value("User_Name", "");
    passwordHash = j.value("Hashed_Password", "");
    email = j.value("Email", "");
    securityQuestion = j.value("Security_Question", "");
    securityAnswer = j.value("Security_Answer", "");
    role = roleFromString(j.value("Role", ""));
}

std::string User::getRoleString() const {
    switch(role) {
        case UserRole::FARMER: return "farmer";
        case UserRole::COOPERATION: return "cooperation";
        default: return "unknown";
    }
}

UserRole User::roleFromString(const std::string& rolestr) {
    if (rolestr == "farmer") return UserRole::FARMER;
    if (rolestr == "cooperation") return UserRole::COOPERATION;
    return UserRole::UNKNOWN;
}

// Session class implementation
Session::Session(const std::string& uname, UserRole r) :
    username(uname),
    role(r) {
    token = securityUtils::generateRandomString(SESSION_TOKEN_LENGTH);
    expiary = time(nullptr) + SESSION_DURATION;
}

nlohmann::json Session::toJson() const {
    return {
        {"User_Name", username},
        {"Token", token},
        {"Expiary_Time", expiary},
        {"Role", User::getRoleString(role)}
    };
}

void Session::fromJson(const nlohmann::json& j) {
    username = j.value("User_Name", "");
    token = j.value("Token", "");
    expiary = j.value("Expiary_Time", 0);
    role = User::roleFromString(j.value("Role", ""));
}

bool Session::isValid() const {
    return time(nullptr) < expiary;
}

// RecoveryToken class implementation
RecoveryToken::RecoveryToken(const std::string& uname, UserRole r) :
    username(uname),
    role(r) {
    token = securityUtils::generateRandomString(RECOVERY_TOKEN_LENGTH);
    expiary = time(nullptr) + RECOVERY_TOKENEXPIARY;
}

nlohmann::json RecoveryToken::toJson() const {
    return {
        {"User_Name", username},
        {"Token", token},
        {"Expiary_Time", expiary},
        {"Role", User::getRoleString(role)}
    };
}

void RecoveryToken::fromJson(const nlohmann::json& j) {
    username = j.value("User_Name", "");
    token = j.value("Token", "");
    expiary = j.value("Expiary_Time", 0);
    role = User::roleFromString(j.value("Role", ""));
}

bool RecoveryToken::isValid() const {
    return time(nullptr) < expiary;
}

// AuthSystem implementation
AuthSystm::AuthSystm() {
    loadFarmers();
    loadCooperations();
    loadSessions();
    loadRecoveryToken();
    cleanupExpiredTokens();
}

AuthSystm::~AuthSystm() {
    saveFarmers();
    saveCooperations();
    saveSessions();
    saveRecoveryToken();
}

void AuthSystm::loadFarmers() {
    farmers = toJsonUtility::readCollectionFromFile<User>(FARMER_FILE);
}

void AuthSystm::saveFarmers() {
    toJsonUtility::writeToFile(FARMER_FILE, toJsonUtility::toJsonCollection(farmers));
}

void AuthSystm::loadCooperations() {
    cooperations = toJsonUtility::readCollectionFromFile<User>(COOPERATIONS_FILE);
}

void AuthSystm::saveCooperations() {
    toJsonUtility::writeToFile(COOPERATIONS_FILE, toJsonUtility::toJsonCollection(cooperations));
}

void AuthSystm::loadSessions() {
    sessions = toJsonUtility::readCollectionFromFile<Session>(SESSIONS_FILE);
}

void AuthSystm::saveSessions() {
    toJsonUtility::writeToFile(SESSIONS_FILE, toJsonUtility::toJsonCollection(sessions));
}

void AuthSystm::loadRecoveryToken() {
    recoveryTokens = toJsonUtility::readCollectionFromFile<RecoveryToken>(RECOVERY_TOKENS_FILE);
}

void AuthSystm::saveRecoveryToken() {
    toJsonUtility::writeToFile(RECOVERY_TOKENS_FILE, toJsonUtility::toJsonCollection(recoveryTokens));
}

std::vector<std::shared_ptr<Session>> AuthSystm::getSessions() const {
    return sessions;
}

std::string AuthSystm::getRoleFilename(UserRole role) const {
    switch(role) {
        case UserRole::FARMER: return FARMER_FILE;
        case UserRole::COOPERATION: return COOPERATIONS_FILE;
        default: return "";
    }
}

std::vector<std::shared_ptr<User>>& AuthSystm::getUserByRole(UserRole role) const{
    switch(role) {
        case UserRole::FARMER: return farmers;
        case UserRole::COOPERATION: return cooperations;
        default: throw std::runtime_error("Unknown UserRole");
    }
}
/*
const std::vector<std::shared_ptr<User>>& AuthSystm::getUsersByRole(UserRole role) const {
    switch(role) {
        case UserRole::FARMER: return farmers;
        case UserRole::COOPERATION: return cooperations;
        default: throw std::runtime_error("Unknown UserRole");
    }
}
*/
void AuthSystm::cleanupExpiredTokens() {
    auto now = time(nullptr);
    
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
        [&now](const auto& s) { return now >= s->expiary; }), sessions.end());
    
    recoveryTokens.erase(std::remove_if(recoveryTokens.begin(), recoveryTokens.end(),
        [&now](const auto& rt) { return now >= rt->expiary; }), recoveryTokens.end());
}

bool AuthSystm::registerUser(const std::string& username, const std::string& password,
                            const std::string& email, const std::string& question,
                            const std::string& answer, UserRole role) {
    // Check if user already exists
    for (const auto& user : farmers) {
        if (user->username == username) return false;
    }
    for (const auto& user : cooperations) {
        if (user->username == username) return false;
    }

    auto newUser = std::make_shared<User>(username, password, email, question, answer, role);
    getUserByRole(role).push_back(newUser);

    switch(role) {
        case UserRole::FARMER: saveFarmers(); break;
        case UserRole::COOPERATION: saveCooperations(); break;
        default: return false;
    }
    return true;
}

std::string AuthSystm::logIn(const std::string& username, const std::string& password) {
    // Check farmers first
    for (const auto& user : farmers) {
        if (user->username == username && securityUtils::verify_password(user->passwordHash, password)) {
            auto session = std::make_shared<Session>(username, UserRole::FARMER);
            sessions.push_back(session);
            saveSessions();
            return session->token;
        }
    }
    
    // Check cooperations
    for (const auto& user : cooperations) {
        if (user->username == username && securityUtils::verify_password(user->passwordHash, password)) {
            auto session = std::make_shared<Session>(username, UserRole::COOPERATION);
            sessions.push_back(session);
            saveSessions();
            return session->token;
        }
    }
    
    return "";
}

bool AuthSystm::isLoggedin(const std::string& token) {
    for (const auto& session : sessions) {
        if (session->token == token && session->isValid()) {
            return true;
        }
    }
    return false;
}

void AuthSystm::logOut(const std::string& token) {
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
        [&token](const auto& s) { return s->token == token; }), sessions.end());
    saveSessions();
}

std::string AuthSystm::getUsername(const std::string& token) {
    for (const auto& session : sessions) {
        if (session->token == token && session->isValid()) {
            return session->username;
        }
    }
    return "";
}

UserRole AuthSystm::getUserRole(const std::string& token) {
    for (const auto& session : sessions) {
        if (session->token == token && session->isValid()) {
            return session->role;
        }
    }
    return UserRole::UNKNOWN;
}

std::string AuthSystm::generateRecoveryToken(const std::string& username) {
    for (const auto& user : farmers) {
        if (user->username == username && !user->email.empty()) {
            auto token = std::make_shared<RecoveryToken>(username, UserRole::FARMER);
            recoveryTokens.push_back(token);
            saveRecoveryToken();
            return token->token;
        }
    }
    
    for (const auto& user : cooperations) {
        if (user->username == username && !user->email.empty()) {
            auto token = std::make_shared<RecoveryToken>(username, UserRole::COOPERATION);
            recoveryTokens.push_back(token);
            saveRecoveryToken();
            return token->token;
        }
    }
    
    return "";
}

bool AuthSystm::verifyRecoveryToken(const std::string& token,const std::string& outUserName, UserRole& outRole) {
    for (const auto& rt : recoveryTokens) {
        if (rt->token == token && rt->isValid()) {
            outUserName = rt->username;
            outRole = rt->role;
            return true;
        }
    }
    return false;
}

bool AuthSystm::resetPasswordWithToken(const std::string& token, const std::string& newPassword) {
    std::string username;
    UserRole role;
    if (!verifyRecoveryToken(token, username, role)) {
        return false;
    }

    auto& users = getUserByRole(role);
    for (auto& user : users) {
        if (user->username == username) {
            user->passwordHash = securityUtils::argon2_hash(newPassword);
            
            switch(role) {
                case UserRole::FARMER: saveFarmers(); break;
                case UserRole::COOPERATION: saveCooperations(); break;
                default: return false;
            }
            
            // Remove the used recovery token
            recoveryTokens.erase(std::remove_if(recoveryTokens.begin(), recoveryTokens.end(),
                [&token](const auto& rt) { return rt->token == token; }), recoveryTokens.end());
            saveRecoveryToken();
            
            return true;
        }
    }
    return false;
}

bool AuthSystm::verifySecurityAnswer(const std::string& username, UserRole role, const std::string& answer) {
    const auto& users = getUserByRole(role);
    for (const auto& user : users) {
        if (user->username == username) {
            return securityUtils::verify_answer(user->securityAnswer, answer);
        }
    }
    return false;
}

bool AuthSystm::resetPasswordWithSecurity(const std::string& username, UserRole role,
                                         const std::string& answer, const std::string& newPassword) {
    auto& users = getUserByRole(role);
    for (auto& user : users) {
        if (user->username == username && securityUtils::verify_answer(user->securityAnswer, answer)) {
            user->passwordHash = securityUtils::argon2_hash(newPassword);
            
            switch(role) {
                case UserRole::FARMER: saveFarmers(); break;
                case UserRole::COOPERATION: saveCooperations(); break;
                default: return false;
            }
            
            return true;
        }
    }
    return false;
}
