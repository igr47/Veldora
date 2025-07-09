#ifndef FARMERS_H
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
		void viewProfile();
		void editProfile();
		void myProduce();
		void addProduceTypes();
		void viewProduceTypes();
		void recordHarvest();
		void editProduceTypes();
		void deleteProduce();
		void viewHarvestHistory();
		void manageInventory();
		void AddItem();
		void viewInventory();
		void updateItemQuantity();
		void recordItemUsage();
		bool setAlertThreshold();
		void checkLowStockAlerts();
		void viewUsageHistory();
		void manageTasks();
		void addTasks();
		void viewTasks();
		void editTasks();
		void markAsComplete();
		bool isComplete();
		void checkOverdueTasks();
};
#endif
