#ifdef STRUCTS_H
#define STRUCTS_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json=nlohmann::json;
struct ProduceRecords{
	std::sting produce_type;
	std::string unit;
	std::string add_timestamp;
	std::string update_timestamp;
	std::vector<double> quantity;
	double average_quantity;
	double total_quantity;
	json toJson() const{
		return{
			{"Produce_Type" , produce_type},
			{"Unit_Of_Measurement" , unit},
			{"Created_At" , add_timestamp},
			{"Updated_At" , update_timestamp},
			{"Produce_Quantity" , quantity},
			{"Total_Quantity" , std::accumulate(quantity.begin(),quantity.end(),0.0)},
			{"Average_Quantity", quantity.empty() ? 0.0 :std::acculate(quantity.begin(),quantity.end(),0.0) /quantity.size()}
		};
	}
	void fromJson(const json& j){
		produce_type=j.value("Produce_Type","");
		unit=j.value("Unit_Of_Measurement","");
		add_timestamp=j.value("Created_At£,"");
		update_timestamp=j.value("Updated_At","");
		for(const auto& val : j.value("Produce_Quantity",json::array())){
			if(val.is_number()){
				quantity.push_back(val);
			}
		}


	
