#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "Accelerometer.h"
#include "Thermometer.h"


//bool comparatorTimestamp(const sensors::ISensor* s1, const sensors::ISensor* s2) {
//	return s1->GetTimestamp() > s2->GetTimestamp();
//}

bool comparatorTimestampISensor(const sensors::ISensor* s1, const sensors::ISensor* s2) {
	return s1->GetTimestamp() > s2->GetTimestamp();
}

int main()
{
	using sensors::ISensor;
	using sensors::SensorData;
	using sensors::SensorType;
	using sensors::ToSensorType;
	using sensors::Accelerometer;
	using sensors::Thermometer;

	std::vector<ISensor*> sensors;

	for (std::ifstream file{ "sensors.log" }; !file.eof(); /* EMPTY */)
	{
		if (!file)
		{
			std::cout << "ifstream not in a good state!";
		}

		std::string id_str, typeStr, timestamp;
		file >> id_str >> typeStr >> timestamp;
		std::uint16_t id{ static_cast<std::uint16_t>(std::stoi(id_str.substr(1, 3))) };
		SensorType type{ ToSensorType(typeStr) };

		float value1, value2, value3;
		std::string status_str;

		if (type == SensorType::ACCM) {
			file >> value1 >> value2 >> value3 >> status_str;
			sensors.push_back(new Accelerometer{ id, type, timestamp,
				status_str == "OK", value1, value2, value3 });
		}
		else {
			file >> value1 >> status_str;
			sensors.push_back(new Thermometer{ id, type, timestamp,
				status_str == "OK", value1 });
		}
	}

	//for (auto sensor : sensors) {
	//	const Thermometer* thermometerPtr = dynamic_cast<const Thermometer*>(sensor);
	//	if (sensor->IsMeasurementValid() 
	//		&& /*sensor->GetType() == SensorType::TEMP*/ thermometerPtr != nullptr
	//		&& 9u <= sensor->GetMonth() && sensor->GetMonth() <= 11u)
	//		std::cout << *sensor << '\n';
	//}

	// In cazul in care folosim ISensor* drept tipul de date al vectorului vom folosi thermometerPtr 
	// pentru a apela functiile de get pentru luna adica:

	for (auto sensor : sensors) {
		const Thermometer* thermometerPtr = dynamic_cast<const Thermometer*>(sensor);
		if (sensor->IsMeasurementValid()
			&& thermometerPtr != nullptr
			&& 9u <= thermometerPtr->GetMonth() && thermometerPtr->GetMonth() <= 11u)
			std::cout << *thermometerPtr << '\n';
	}

	std::sort(sensors.begin(), sensors.end(), comparatorTimestampISensor);

	std::cout << "Sortarea vectorului de senzori: \n";

	for (auto s : sensors)
		std::cout << *dynamic_cast<SensorData*>(s) << '\n';



	return 0;
}

