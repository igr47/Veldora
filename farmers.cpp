#include "farmers.h"
#include "structs.h"
#include <nlohmann/json.hpp>
using json=nlohmann::json;

class toJsonUtility;
class AuthSystm;
class TimestampManager;


struct Farmers::farmersInfo{
	struct farmer{
		std::string username;
		std::string first_name;
		std::string second_name;
		std::string farm_location;
		std::string farm_name;
		std::string operations_describtion;
		int contact_number;
		int year_of_birth;
		std::vector<ProduceRecords> produce;
		std::vector<InventoryItem> inventory;
		json toJson() const{
			json j;
			j["UserName"]=username;
			j["First_Name"]=first_name;
			j["Second_Nme"]=second_name;
			j["Year_Of_Birth"]=year_of_birth;
			j["Location"]=farm_location;
			j["Farm_Name"]=farm_name;
			j["About_Farm"]=operations_describtion;
			j["Contact"]=contact_number;
			json produceArray=json::array();
			for(const auto& prod : produce){
				produceArray.push_back(prod);
			}
			j["Produce_Records"]=produceArray;
			json inventoryArray=json::array();
			for(const auto& inv : inventory){
				inventoryArray.push_back(inv);
			}
			j["Inventory"]=inventoryArray;
		}
		void fromJson(const json& j){
			username=j.value("UserName","");
			first_name=j.value("First_Name","");
			second_name=j.value("Second_Name","");
			year_of_birth=j.value("Year_Of_Birth",0);
			farm_location=j.value("Location","");
			farm_name=j.valur("Farm_Name","");
			operatons_describtion=j.value("About_Farm","");
			contact_number=j.value("Contact","");
			produce.clear();
			if(j.contains("Produce_Records") && j["Produce_Records"].is_array()){
				for(const auto& item : j["Produce_Records"]){
					ProduceRecords p;
					p.fromJson(item);
					produce.push_back(p);
				}
			}
			inventory.clear();
			if(j.contains("Inventory") && j["Inventory"].is_array()){
				for(const auto& item : j["Inventory"]){
					InventoryItem i;
					i.fromJson(item);
					inventory.push_back(i);
				}
			}
		}

	};
	std::vector<std::shared_ptr<farmer>> farmersList;

};

void Farmers::loadFarmer(){
	frm->farmersList=toJsonUtility::readCollectionFromFile<Farmers::farmersInfo>("farmers.json");
}
void Farmers::saveFarmer(){
	toJsonUtility::writeToFile("farmersJson",toJsonCollection(frm->farmersList));
}

// constructors for the class
// i will initilize the save and load details so that they are created and saved in every construction and destruction
Farmers::Farmers() : frm(std::make_shared<farmersInfo>()){
	loadFarmer();
}
Farmers::~Farmers(){
	saveFarmer();
}
namespace sharedUtils{
	bool isCurrent(){
		for(const auto& user : frm->farmersList){
			if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
				return true;
			}
			return false;
		}
	}
}
void Farmers::myProfile(){
	std::cout<<"\n========MY PROFILE=========\n";
	int choise;
	std::cout<<"\nWould you like to? ";
	std::cout<<"\n1.Create my profile. ";
	std::cout<<"\n2.View my profile. ";
	std::cout<<"\n3.Edit my profile. ";
	std::cout<<"\n4.Exit! ";

	switch(choise){
		case 1; createProfile(); break;
		case 2: viewProfile(); break;
		case 3: editProfile(); break;
		case 4: break;
		default: std::cout<<"\nPlease choose the option between (1-4)";
	}
}

void Farmers::createProfile(){
	std::cout<<"\n========CRAETE MY PROFILE========\n";
	Farmers::farmerInfo::farmer newFarmer;
	std::cout<<"\nEnter your details";
	std::cout<<"\nUserName(Should be the one used for login): ";
	std::getline(std::cin,newFarmer.username);
	std::cout<<"\nFirst name: ";
	std::getline(std::cin,newFarmer.first_name);
	std::cout<<"\nSecond name: ";
	std::getline(std::cin,newFarmer.second_name);
	std::cout<<"\nLocation: ";
	std::getline(std::cin,newFarmer.farm_location);
	std::cout<<"\nFarm name: ";
	std::getline(std::cin,newFarmer.farm_name);
	std::cout<<"\nFarme describtion: ";
	std::getline(std::cin,newFarmer.operations_describtion);
	std::cout<<"\nYear of birth: ";
	std::cin>>newFarmer.year_of_birth;
	std::cin.ignore();
	std::cout<<"\nContact: ";
	std::cin>>newFarmer.contact_number;
	std::cin.ignore();
	frm->farmersList.push_back(std::make_shared<farmerInfo::farmer>(newFarmer));
	saveFarmer();
	std::cout<<"\nYour details were saved successively.";
}
//Method to view the current saved details

void Farmers::viewProfile(){
	for(const auto& user : frm->farmersList){
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n========PROFILE VIEWER========\n";
			std::cout<<"\nYour profile is: ";
			std::cout<<"\n----------------------------------------------\n";
			std::cout<<"\nUserName: "<< user.username
				<<"\nFirst_Name: "<< user.first_name
				<<"\nSecond_Name: "<< user.second_name
				<<"\nYear_Of_Birth: "<< user.year_of_birth
				<<"\nLocation: "<< user.farm_location
				<<"\nFarm_Name: "<< user.farm_name
				<<"\nAbout_Farm: "<< user.operations_describtion
				<<"\nContact: "<< user.contact_number
				<<"\n";
			std::cout<<"\n-----------------------------------------------\n";
		}else{
			std::cout<<"\nSorry but you were not found in the system.Creat profile to view.";
		}
	}
}
//method to edit the users profile
void Farmers::editProfile(){
	if(sharedUtils::isCurrent()){
		std::cout<<"\n=======EDIT PROFILE==========\n";
		std::cout<<"\nYour current details are: ";
		viewProfile();
		std::cout<<"\nEnter your new details: ";
		std::cout<<"\nFirst_name: ";
		std::getline(std::cin,farmersList.first_name);
		std::cout<<"\nSecond_Name: ";
		std::getline(std::cin,farmersList.second_name);
		std::cout<<"\nLocation: ";
		std::getline(std::cin,farmersList.farm_location);
		std::cout<<"\nFarm_name: ";
		std::getline(std::cin,farmerList.farm_name);
		std::cout<<"\nAbout_Farm: ";
		std::getline(std::cin,farmersList.operations_describtion);
		std::cout<<"\nYear_Of_Birth: ";
		std::cin>>year_of_birth;
		std::cin.ignore();
		std::cout<<"\nContact: ";
		std::cin>>contact_number;
		std::cin.ignore();
		saveFarmer();
	}else{
		std::cout<<"\nSorry! Your details were not found in the database.Create profile to edit.";
	}
}
//**************************************************************************************************************
//I am done with the profile classese;
//***********************************************************************************************************
//I am now starting the  produce methods
void myProduce(){
	int choise;
	std::cout<<"\n========MY PRODUCE MANAGER============";
	std::cout<<"\nWould you like to: ";
	std::cout<<"\n1.Add produce types.";
	std::cout<<"\n2.View produce types. ";
	std::cout<<"\n3.Record Harvest.";
	std::cout<<"\n4.Edit produce type details.";
	std::cout<<"\n5.Delete produce.";
	std::cout <<"\n6.View harvest history.";
	std::cout<<"\n7.Exit.";
	std::cin>>choise;
	std::cin.ignore();
	switch(choise){
		case 1: addProduceTypes(); break;
		case 2: viewProduceTypes(); break;
		case 3: recordHarvest(); break;
		case 4: editProduceType(); break;
		case 5: deleteProduce(); break;
		case 6: viewHarvetHistory(); break;
		case 7: break;
		default: std::cout<<"\nPlease enter one of the options above."; break;
	}
}
 void Farmers::addProduceTypes(){
	 for(const auto& user : frm->farmersList){
                if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n======ADD PRODUCE========\n";
			ProduceRecord newProduce;
			std::string choise;
			do{
				std::cout<<"\nEnter produce:";
				std::cout<<"\nProduce type: ";
				std::getline(std::cin,newProduce.produce_type);
				std::cout<<"\nUnit of measurement: ";
				std::getline(std::cin,newProduce.unit);
				newProduce.add_timestamp=TimestampManager::createTimestamp();
				newProduce.quantity=0.0;
				newProduce.update_timestamp=TimestampManager::createTimestamp();
				newProduce.total_quantity=0.0;
				newProduce.average_quantity=0.0;
				user.produce.push_back(newProduce);
				saveFarmer();
				std::cout<<"\nNew produce added successively.";
				std::cout<<"\nWould you like to add another produce type(Y/N?";
				std::getline(std::cin,choise);
			}while(choise=="y" || choise=="Y");
		}else{
			std::cout<<"\nSorry. You were not found in the system.";
		}
	}
}

void Farmers::viewProduceTypes(){
	for(const auto& user : frm->farmersList){       
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n==============MY PRODUCE TYPES=============\n";
			std::cout<<"\nThe produce types you deal in are:";
			std::cout<<std::left<<std::setw(20)<<"Produce_Type: "
				<<std::setw(15)<<"Unit Of Measurement: "
				<<std::setw(15)<<"Created_At: "
				<<std::setw(15)<<"Updated_At: "
				<<"\n----------------------------------------\n";
			for(const auto& item : user.produce){
				std::cout<<std::left<<std::setw(20)<<item.produce_type
					<<std::setw(15)<<item.unit
					<<std::setw(15)<<item.add_timestamp
					<<std::setw(15)<<item.update_timestamp;
			}
		}else{
			std::cout<<"\nSorry.You were not found in the system!";
		}
	}
}

void Farmers::recordHarvest(){
	for(const auto& user : frm->farmersList){
                if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n=========RECORD MY PRODUCE=========\N";
			std::string choise;
			std::string record;
			do{
				std::cout<<"\nEnter the produce you wish to record: ";
				std::getline(std::cin,record);
				auto it=std::find_if(user.produce.begin(),user.produce.end(),[&record](const std::shared_ptr<ProduceRecord>& p){
						return p->produce_type==record;
					});
				if(it!=user.produce.end()){
					double quantity;
					std::string type;
					std::string time;
					std::string unit;
					std::cout<<"\nEnter  quantity: ";
					std::cin>>quantity;
					std::cin.ignore();
					std::cout<<"Unit(kgs/ltrs/gms): ";
					std::getline(std::cin,unit);
					type=record;
					time=TimestampManager::createTimestamp();
					(*it)->harvestHistory.emplace_back(type,unit,time,quantity);
					saveFarmer();
				}else{
					std::cout<<"\nThe produce was not found.Please add to the produce types";
				}
				std::cout<<"\nWould you wish to record another produce(y/n)? ";
				std::getline(std::cin,choise);
			}while(choise=="Y" || choise=="y");
		}else{
			std::cout<<"\nSorry.you were not found in the system.";
		}
	}
}

void Farmers::editProduceType(){
	for(const auto& user : frm->farmersList){      
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n============EDIT MY PRODUCE============\n";
			viewProduceTypes();
			std::string choise;
			do{
				std::string edit;
				std::cout<<"\nEnter the name of the produce type you wish to edit: ";
				std::getline(std::cin,edit);
				auto it=std::find_if(user.produce.begin(),user.produce.end(),[&edit](const std::shared_ptr<ProduceRecords>& r){
						return r->produce_type==edit;
					});
				if(it!=user.prodece.end()){
				        std::cout<<"\nNew produce details: ";
				        std::cout<<"\nProduce name: ";
				        std::getline(std::cin,(*it)->produce_type);
					std::cout<<"Unit Of Measurements: ";
					std::getline(std::cin,(*it)->unit);
					(*it)->update_timestamp=TimestampManager::createTimestamp();
					saveFarmer();
				}else{
					std::cout<<"\nProduce not found!!";
				}
				std::cout<<"\nWould you like to edit another produce(y/n)? ";
				std::getline(std::cin,choise);
			}while(choise=="y" || choise=="Y");
		}else{
			std::cout<<"\nSorry.You were not found in the system!!";
		}
	}
}

void Farmers::deleteProduce(){
	for(const auto& user : frm->farmersList){       
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n==========DELETE PRODUCE TYPE=============\n";
			viewProduceTypes();
			std::string choise;
			do{
				std::string del;
				std::cout<<"\nEnter the name of the produce you wish to delete: ";
				std::getline(std::cin,del);
				std::erase(std::remove_if(user.produce.begin(),user.produce.end(),[&del](const ProduceRecords& r){
							return  r.produce_type==del;}),user.produce.end());
				saveFarmer();
				std::cout<<"\nProduce successively deleted.";
				std::cout<<"\nWould you like to delete another item(y/n)? ";
				std::getline(std::cin,choise);
			}while(choise=="y" || choise=="Y");
		}else{
			std::cout<<"\nSorry but your details were not found in the database!!";
		}
	}
}

void Farmers::viewHarvestHistory(){
	for(const auto& user : frm->farmersList){     
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n=========VIEW PRODUCE HISTORY=========\n";
			int choise;
			std::cout<<"\nWould you like to view history of: "
				<<"\n1.One produce"
				<<"\n2.All produce"
				<<"\n";
			std::cin>>choise;
			std::cin.ignore();
			if(choise=1){
				viewProduceTypes();
				std::string choise;
				do{
					std::string option;
					std::cout<<"\nEnter the name of the produce you wish to view: ";
					std::getline(std::cin,option);
					auto it=std::find_if(user.produce.begin(),user.produce.end(),[&option](const ProduceRecord& p){
							return p.produce_type==option;
						});
					if(it!=user.produce.end()){
						std::cout<<"\nThe records are:";
						std::cout<<std::left
							<<std::setw(20)<<"Produce_Type: "
							<<std::setw(15)<<"Quantity: "
							<<std::setw(15)<<"Units: "
							<<std::setw(20)<<"Recorded_At: "
							<<"\n---------------------------------------------\n";
						for(const auto& item : (*it)->harvestHistory){
							if(item==option){
								std::cout<<std::left
									<<std::setw(20)<<item.produceType
									<<std::setw(15)<<item.quantity
									<<std::setw(15)<<item.unit
									<<std::setw(20)<<item.record_timestamp;
							}
						}
					}else{
						std::cout<<"\nProduce you searched for not found!";
					}
					std::cout<<"\nWould you like to view another produce type(y/n)? ";
					std::getline(std::cin,choise);
				}while(choise=="y" || choise=="Y");
			}else(choise==2){
				std::cout<<"\nThe records are:\n";
                                std::cout<<std::left
                                        <<std::setw(20)<<"Produce_Type: "
                                        <<std::setw(15)<<"Quantity: "
                                        <<std::setw(15)<<"Units: "
                                        <<std::setw(20)<<"Recorded_At: "   
					<<"\n---------------------------------------------\n";
				for(const auto& item :  user.produce.harvestHistory){
					std::cout<<std::left
                                                <<std::setw(20)<<item.produceType
                                                <<std::setw(15)<<item.quantity  
						<<std::setw(15)<<item.unit    
						<<std::setw(20)<<item.record_timestamp;    
				}
			}
		}else{
			std::cout<<"\nSorry but you were not found in the database!!";
		}
	}
}
//************************************************************************************
//Starting working on inventory management and record keeping
//************************************************************************************

void Farmers::manageInventory(){
	int choise;
	do{
		std::cout<<"\n===========INVENTORY MANAGEMENT=============\n";
		std::cout<<"\m1.Add new item: "
			<<"\n2.View inventory: "
			<<"\n3.Update item quantity: "
			<<"\n4.Record item usage: "
			<<"\n5.Check low stock Alerts: "
			<<"\n6.View usage history: "
			<<"\n0.Return to main menu: "
			<<"\nEnter your choise: ";
		std::cin>>choise;

		switch(choise){
			case 1:AddItem(); break;
			case 2: viewInventory(); break;
			case 3: updateItemQuantity(); break;
			case 4: recordItemUsage(); break;
			case 5: checkLowStockAlerts(); break;
			case 6: viewUsageHistory(); break;
		}
	}while(choise!=0);
}

void Farmers::AddItem(){
	for(const auto& user : frm->farmersList){   
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n==========ADD ITEM============\n";
			std::string choise;
			do{
				InventoryItem newItem;
				std::cout<<"\nEnter details for new: ";
				std::cout<<"\nType of item(Cow Feed,Seeds,Fertilizer,etc): ";
				std::getline(std::cin,newItem.type);
				std::cout<<"\nName of iem(Dairy Meal,Maize Germ): ";
				std::getline(std::cin,newItem.name);
				std::cout<<"\nDescribtion: ";
				std::getline(std::cin,newItem.describtion);
				std::cout<<"\nQuantity: ";
				std::cin>>newItem.quantity;
				std::cin.ignore();
				std::cout<<"\nUnit: ";
				std::getline(std::cin,newItem.unit);
				std::cout<<"\nAlert Threshold: ";
				std::getline(std::cin,newItem.alertThreshold);
				newItem.date_of_entry=TimestampManager::createTimestamp();
				newItem.date_of_update=TimestampManager::createTimestamp();
				saveFarmer();
				std::cout<<"\nYour details were saved successively.";
				std::cout<<"\nWould you like to add another item(y/n)? ";
				std::getline(std::cin,choise);
			}while(choise=="y" || choise=="Y");
		}else{
			std::cout<<"\nSorry but your details were not found in the database!";
		}
	}
}
 void Farmers::viewInventory(){
	 for(const auto& user : frm->farmersList){ 
		 if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			 std::cout<<"\==========MY INVENTORY=============\N";
			 std::cout<<std::left<<std::setw(20)<<"Type of produce: "
				 <<std::setw(20)<<"Name of produce: "
				 <<std::setw(20)<<"Describtion: "
				 <<std::setw(15)<<"Quantity: "
				 <<std::setw(15)<<"Unit: "
				 <<std::setw(15)<<"Alert Threshold:"
				 <<std::setw(20)<<"Date Of Entry: "
				 <<std::setw(20)<<"Date Of Update: "
				 <<"\n-----------------------------------------------\n";
			 for(const auto& item : user.inventory){
				 std::cout<<std::left<<std::setw(20)<<item.type
					 <<std::setw(20)<<item.name
					 <<std::setw(20)<<item.describtion
					 <<std::setw(15)<<item.quantity
					 <<std::setw(15)<<item.unit
					 <<std::setw(15)<<item.alertThreshold
					 <<std::setw(20)<<item.date_of_entry
					 <<std::setw(20)<<item.date_of_update;
			}
		}else{
			std::cout<<"\nSorry but your details were not found in the database!!";
		}
	}
}

void Faremrs::updateItemQuantity(){
	for(const auto& user : frm->farmersList){   
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\=========UPDATE QUANTITY==========\n";
			viewInventory();
			std::string choise;
			do{
				std::string option;
				std::cout<<"\nEnter the name of the item to update(Maize germ,Broiler Starter,Tractors): ";
				std::getline(std::cin,option);
				auto it=std::find_if(user.inventory.begin(),user.inventory.end(),[&option](const InventoryItem& i){
						return i.name==option;
					});
				if(it!=user.inventory.end()){
					std::cout<<"\nEnter new quantity for " <<option<< ": ";
					std::getline(std::cin,(*it)->quantity);
					saveFarmer();
					std::cout<<"\nQuamtity successively updated.";
					std::cout<<"\nWould you like to update another item(y/n)? ";
					std::getline(std::cin,choise);
				}else{
					std::cout<<"\nItem not found in the inventory.";
				}
			}while(choise=="y" || choise=="Y");
		}else{
			std::cout<<"\nSorry but your details were not found in the system!!";
		}
	}
}

void Farmers::recordItemUsage(){
	for(const auto& user : frm->farmersList){
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n==========USAGE RECORDING==========\n;
			std::string choise;
			do{
				std::string option;
				std::cout<<"\nEnter the name of item you wish to use: ";
				std::getline(std::cin,option);
				auto it=std::find_if(user.inventory.begin(),user.inventory.end(),[&option](const InventoryItem& i){
								return i.name==option;
							});
				if(it!=user.inventory.end()){
					std::string name;
					std::string purpose;
					std::string date_of_use;
					std::string employeeusing;
					double amount_used;
					std::cout<<"\nEnter the: ";
					std::cout<<"\nItem: ";
					std::getline(std:;cin,name);
					std::cout<<"\nPurpose: ";
					std::getline(std::cin,purpose);
					std::cout<<"\nEmployee Using: ";
					std::getline(std::cin,employeeusing);
					std::cout<<"\nAmount Used: ";
					std::cin>>amount_used;
					std::cin.ignore();
					date_of_use=TimestampManager::createTimestamp();
					(*it)->quantity-=amount_used;
					(*it)->history.emplce_back(name,purpose,date_of_use,employeeusing,amount_used);
					saveFarmer();
					std::cout<<"\nUsage historu updated.";
					std::cout<<"\nWould you like to record another item(y/n)? ";
					std::geline(std::cin,choise);
				}else{
					std::cout<<"\nItem not found!!";
				}
			}while(choise=="y" || choise=="Y");
		}else{
			std::cout<<"\nSorry but your details were not found.";
		}
	}
}

bool Farmers::setAlertThreshold(){
	for(const auto& user : frm->farmersList){    
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			for(const auto& item : user.inventory){
				if(item.quantity <= item.alertThreshold){
					std::cout<<"\n"<<item.name <<"Is below alert threshold. PLease top up amount to avoid scarcity";
					return true;
				}
				return false;
			}
		}else{
			std::cout<<"\nSorry but details not found,";
		}
	}
}

void Farmers::checklowStocAlerts(){
	for(const auto& user : frm->farmersList){ 
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"\n==============LOW STOCK ALERTS============\n";
			setAlertThreshold();
		}else{
			<<"\nSorry your details were not found.";
		}
	}
}

void Farmers::viewUsageHistory(){
	for(const auto& user : frm->farmersList){     
		if(user.username==AuthSystm::getUsername(AuthSystm::getSessions().front())){
			std::cout<<"==============USAGE HISTORY===================\n";
			std::cout<<"\nWould you like to view usage history for:";
			std::cout<<"\n1.One item: ";
			std::cout<<"\n2.All items: ";
			int opt;
			std::cin>>opt;
			std::cin.ignore();
			if(opt==1){
				viewInventory();
				std::string choise;
				do{
					std::string option;
					std::cout<<"\nEnter the name of the item you wish to view history: ";
					std::getline(std::cin,option);
					auto it=std::find_if(user.inventory.begin(),user.inventory.end()[&option](const InventoryItem& i){
							return i.name==option;
						});
					std::cout<<std::left<<std::setw(20)<<"Item: "
						<<std::setw(15)<<"Amount_Used: "
						<<std::setw(20)<<"Purpose: "
						<<std::setw(20)<<"Date Used: "
						<<std::setw(20)<<"Employee Using: "
						<<"\n--------------------------------------\n";
					for(const auto& item : user.inventory){
						if(item.name==(*it)->name){
							std::cout<<std::left<<std::setw(20)<<item.name
								<<std::setw(15)<<item.amount_used
								<<std::setw(20)<<item.purpose
								<<std::setw(20)<<item.date_used
								<<std::setw(20)<<item.employee_using;
						}
					}
					std::cout<<"\nWould you like to view history of another produce(y/n)? ";
					std::getline(std::cin,choise);
				}while(choise=="y" || choise=="Y");
			}else(opt==2){
				std::cout<<std::left<<std::setw(20)<<"Item: "
                                        <<std::setw(15)<<"Amount_Used: " 
					<<std::setw(20)<<"Purpose: "   
					<<std::setw(20)<<"Date Used: "
                                        <<std::setw(20)<<"Employee Using: "
                                        <<"\n--------------------------------------\n";
				for(const auto& item : user.inventory){
					std::cout<<std::left<<std::setw(20)<<item.name
						<<std::setw(15)<<item.amount_used
                                                <<std::setw(20)<<item.purpose
                                                <<std::setw(20)<<item.date_used
                                                <<std::setw(20)<<item.employee_using;     
				}
			}
		}else{
			std::cout<<"\nSorry but your details were not found in the database.";
		}
	}
}


				








				

					



			







	 



