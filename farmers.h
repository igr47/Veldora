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
		namespace sharedUtils{
			bool isCurrent();
		}
		Farmers();
		~Farmers();
		void myProfile();
		void createProfile();
		void viewProfile();
		void editProfile();
		void myProduce();
		void addProduceTypes();
		void viewProduceTypes();
		void viewHarvest();
		void editProduceTypes();
		void deleteProduce();
		void viewHarvestHistory();
