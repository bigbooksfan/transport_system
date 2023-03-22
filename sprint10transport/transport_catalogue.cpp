#include "transport_catalogue.h"

#include <stdexcept>
#include <algorithm>

#include "input_reader.h"

transport_catalogue::transport_catalogue(std::string& number) {
	InputReader::input_reader reader(std::stoi(number), this);
	ParseRoutes();
}

void transport_catalogue::AddBus(const std::string& name, const std::string& raw_desc) {
	if (buses_.count(name) != 0)
		throw std::logic_error("Bus name is not unique");

	buses_.insert({ name, raw_desc });
}

void transport_catalogue::AddStop(const std::string& name, const Coordinates& coords) {
	if (stops_.count(name) != 0)
		throw std::logic_error("Stop name is not unique");

	stops_.insert({ name, coords });
}

const std::string& transport_catalogue::FindBus(const std::string& name) const {
	if (buses_.count(name) == 0)
		throw std::logic_error("Search of non-existent bus");
	return buses_.at(name);
}

const Coordinates& transport_catalogue::FindStop(const std::string& name) const {
	if (stops_.count(name) == 0)
		throw std::logic_error("Search of non-existent stop");
	return stops_.at(name);
}

void transport_catalogue::ParseRoutes() {
	for (const auto& bus : buses_) {
		if (routes_.count(bus.first) != 0)
			throw std::logic_error("Bus name is not unique");

		if (		// circular route
			bus.second.find(BusCircleDelimeter) != std::string::npos &&
			bus.second.find(BusLineDelimeter) == std::string::npos) {
			
			routes_.insert({ bus.first, ParseCircularRoute(bus.second)});
			continue;
		}

		if (		// linear route
			bus.second.find(BusCircleDelimeter) == std::string::npos &&
			bus.second.find(BusLineDelimeter) != std::string::npos) {

			routes_.insert({ bus.first, ParseLinearRoute(bus.second) });
			continue;
		}

		throw std::logic_error("Raw route is not linear or circular or both");
	}
}

transport_catalogue::Route transport_catalogue::ParseLinearRoute(
	const std::string& raw_desc) {

	Route ret;
	std::vector<size_t> delim_pos;

	// need to reforge it
	for (size_t i = 0, j = raw_desc.size(); i < j; ++i) {
		if (raw_desc[i] == BusLineDelimeter) {
			++ret.unique_stops_;
			delim_pos.push_back(i);
		}
	}

	return Route();
}

transport_catalogue::Route transport_catalogue::ParseCircularRoute(
	const std::string& raw_desc) {
	return Route();
}
