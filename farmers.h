#ifdef FARMERS_H
#define FARMERS_H
#include <string>
#include <vector>
#include <memory>

class Farmers{
	private:
		struct farmersInfo;
		std::shared_ptr<farmersInfo> frm;
		void loadFarmer();
		void saveFarmer();
	public:
		Farmers();
		~Farmers();
		void myProfile();
		void createProfile();
