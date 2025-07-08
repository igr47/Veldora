#ifdef STRUCTS_H
#define STRUCTS_H
#include <string>
#include <vector>
#include <numeric>
#include <nlohmann/json.hpp>

using json=nlohmann::json;
struct ProduceRecords{
	struct HarvestHistory{
		std::string produceType;
		std::string unit;
		std::string record_timestamp;
		std::vector<double> quantity;
		json toJson() const{
			return{
				{"Produce_Type",produceType},
				{"Unit",unit},
				{"Time_Recorded", record_timestamp},
				{"Quantity" , quantity}
			};
		}
		void fromJson(const json& j){
			produceType=j.value("Produce_Type","");
			unit=j.value("Unit","");
			record_timestamp=j.value("Time_Recorded","");
			for(const auto& val : j.value("Quantity",json::array())){
				if(val.is_number()){
                                        quantity.push_back(val);
                                }
                        }		

	        }
	};
	std::string produce_type;
	std::string unit;
	std::string add_timestamp;
	std::string update_timestamp;
	std::vector<double> quantity;
	std::vector<HarvestHistory> harvestHistory;
	double average_quantity;
	double total_quantity;
	json toJson() const{
		json harvestHistoryJson;
		for(const auto& history : harvestHistory){
			harvestHistoryJson.push_back(history.toJson());
		}

		return{
			{"Produce_Type" , produce_type},
			{"Unit_Of_Measurement" , unit},
			{"Created_At" , add_timestamp},
			{"Updated_At" , update_timestamp},
			{"Produce_Quantity" , quantity},
			{"Total_Quantity" , std::accumulate(quantity.begin(),quantity.end(),0.0)},
			{"Average_Quantity", quantity.empty() ? 0.0 :std::accumulate(quantity.begin(),quantity.end(),0.0) /quantity.size()},
			{"Harvest_History",harvestHistoryJson}
		};
	}
	void fromJson(const json& j){
		produce_type=j.value("Produce_Type","");
		unit=j.value("Unit_Of_Measurement","");
		add_timestamp=j.value("Created_At,"");
		update_timestamp=j.value("Updated_At","");
		for(const auto& val : j.value("Produce_Quantity",json::array())){
			if(val.is_number()){
				quantity.push_back(val);
			}
		}
		harvestHistory.clear();
		for(const auto& historyJson : j.value("Harvest_History",json::array())){
			HarvestHistory history;
			history.fromJson(historyJson);
			harvestHistory.push_back(history);
		}
	}
};
//Struct for inventory of items
struct InventoryItem {
    struct UsageHistory{
	    std::string name;
	    std::string purpose;
	    std::string date_used;
	    std::string employee_using;
	    double amount_used;
	    json toJson() const{
		    return{
			    {"Item" , name},
			    {"Purpose_Of_Item", purpose},
			    {"Date_Used" , date_used},
			    {"Employee_Using", employee_using},
			    {"Amount_Used" ,amount_used}
			};
	    }
	    void fromJson(const json& j){
		    name=j.value("Item","');
		    purpose=j.value("Purpose_Of_Item","");
		    date_used=j.value("Date_Used","");
		    employee_using=j.value("Employee_Using","");
		    amount_used=j.value("Amount_Used","");
	    }
    };

    std::string name; // "Maize Germ",,"Chicken Starter","Tractors"
    std::string type; // "Seed", "Fertilizer", "Equipment", "CowFeed","ChickenFeed",etc.
    std::string descrition;
    std::string date_of_entry;
    std::string date_of_uodate;
    double quantity; // "90",etc
    std::string unit; // "kg", "liters", "units", etc.
    double alertThreshold; // Minimum quantity before alert 
    std::vector<UsageHistory> history;
    json toJson() const { 
	    json usageHistoryJson;
	    for(const auto& his : history){
		    usageHistoryJson.push_back(his.toJson());
	    }
	    return {                                                             
		    {"Name", name},
                    {"Type", type},
		    {"Describtion",describtion},
		    {"Date_Of_Entry",date_of_entry},
		    {"Date_Of_Update",date_of_update},
                    {"Quantity", quantity},
                    {"Unit", unit},
                    {"AlertThreshold", alertThreshold},
                    {"UsageHistory", usageHistoryJson}
          };
    }

    void fromJson(const json& j) {
        name = j.value("Name", "");
        type = j.value("Type", "");
	describtion=j.value("Describtion","");
	date_of_entry=j.value("Date_Of_Entery","");
	date_of_update=j.value("Date_Of_Update","");
        quantity = j.value("Quantity", 0.0);
        unit = j.value("Unit", "");
        alertThreshold = j.value("AlertThreshold", 0.0);
        history.clear();
	for(const auto& historyJson : j.value("UsageHistory",json::array())){
		UsageHistory hist;
		hist.fromJson(historyJson);
		history.push_back(hist);
	}
    }
};


	
