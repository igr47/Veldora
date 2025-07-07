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


	
