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
	const Stop* tmp = &*stops_.find(name);
	return tmp->coords_;
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

	size_t j = raw_desc.size();
	Route ret;
	ret.way_.reserve(j / 5);
	size_t bgn = 0, end = 0, sz = 0;

	for (size_t i = 0; i < j; ++i) {
		if (raw_desc[i] == BusLineDelimeter || i == j - 1) {
			++ret.unique_stops_;
			end = (i == j - 1) ? j + 1 : i;
			sz = end - bgn - 1;
			ret.way_.push_back({&raw_desc[bgn], sz});
			bgn = end + 2;
		}
	}

	// adding tail
	ret.way_.reserve(ret.way_.size() * 2);
	ret.way_.insert(ret.way_.end(), ret.way_.rbegin() + 1, ret.way_.rend());

	ret.way_.shrink_to_fit();
	//ret.length = ret.way_.size();	// some method
	return ret;
}

transport_catalogue::Route transport_catalogue::ParseCircularRoute(
	const std::string& raw_desc) {

	size_t j = raw_desc.size();
	Route ret;
	ret.way_.reserve(j / 5);
	size_t bgn = 0, end = 0, sz = 0;



	return ret;
}

void transport_catalogue::CalcDistances(Route& route) {

}

bool transport_catalogue::Stop::operator<(const Stop& other) const {
	return name_ < other.name_;
}

bool transport_catalogue::Stop::operator==(const Stop& other) const {
	return name_ == other.name_;
}
