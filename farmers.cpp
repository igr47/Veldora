#include "farmers.h"
#include <nlohmann/json.hpp>
using json=nlohmann::json;

class toJsonUtility;



struct Farmers::farmersInfo{
	struct farmer{
		std::string first_name;
		std::string second_name;
		std::string farm_location;
		std::string farm_name;
		std::string operations_describtion;
		int contact_number;
		int year_of_birth;
		json toJson() const{
			json j;
			j["First_Name"]=first_name;
			j["Second_Nme"]=second_name;
			j["Year_Of_Birth"]=year_of_birth;
			j["Location"]=farm_location;
			j["Farm_Name"]=farm_name;
			j["About_Farm"]=operations_describtion;
			j["Contact"]=contact_number;
		}
		void fromJson(const json& j){
			first_name=j.value("First_Name","");
			second_name=j.value("Second_Name","");
			year_of_birth=j.value("Year_Of_Birth",0);
			farm_location=j.value("Location","");
			farm_name=j.valur("Farm_Name","");
			operatons_describtion=j.value("About_Farm","");
			contact_number=j.value("Contact","");
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
	if(frm->farmersList.empty()) break;
	Farmers::farmerInfo::farmer newFarmer;
	std::cout<<"\nEnter your details";
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
	std::cin>>newFarmeryear_of_birth;
	std::cin.ignore();
	std::cout<<"\nContact: ";
	std::cin>>newFarmer.

