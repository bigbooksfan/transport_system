#include "transport_catalogue.h"

#include <stdexcept>
#include <algorithm>

#include "input_reader.h"

transport_catalogue::transport_catalogue(std::string& number) {
	InputReader::input_reader reader(std::stoi(number), this);
	ParseRoutes();
}

transport_catalogue::transport_catalogue(size_t number) {
	InputReader::input_reader reader(number, this);
	ParseRoutes();
}

void transport_catalogue::AddStop(const std::string& name, const double lat, const double lon) {
	if (stops_.count(name) != 0)
		throw std::logic_error("Stop name is not unique");
	stops_.insert(Stop{ name, { lat, lon} });
}

void transport_catalogue::AddBus(const std::string& name, const std::string& raw_desc) {
	if (buses_.count(name) != 0)
		throw std::logic_error("Bus name is not unique");
	buses_.insert(Bus{ name, raw_desc });
}

const transport_catalogue::Stop& transport_catalogue::FindStop(
	const std::string& name) const {
	if (stops_.count(name) == 0)
		throw std::logic_error("Search of non-existent stop");
	return *stops_.find(name);
}

const transport_catalogue::Bus& transport_catalogue::FindBus(
	const std::string& name) const {
	if (buses_.count(name) == 0)
		throw std::logic_error("Search of non-existent bus");
	return *buses_.find(name);
}

void transport_catalogue::ParseRoutes() {
	for (const auto& bus : buses_) {
		if (routes_.count(bus.name_) != 0)
			throw std::logic_error("Bus name is not unique");

		if (		// circular route
			bus.raw_route_.find(BusCircleDelimeter) != std::string::npos &&
			bus.raw_route_.find(BusLineDelimeter) == std::string::npos) {
			
			dummy_ = ParseCircularRoute(bus.raw_route_);
			routes_.insert({ bus.name_, dummy_});
			continue;
		}

		if (		// linear route
			bus.raw_route_.find(BusCircleDelimeter) == std::string::npos &&
			bus.raw_route_.find(BusLineDelimeter) != std::string::npos) {

			dummy_ = ParseLinearRoute(bus.raw_route_);
			routes_.insert({ bus.name_, dummy_ });
			continue;
		}

		throw std::logic_error("Raw route is both linear and circular or none of them");
	}
}

transport_catalogue::Stop* transport_catalogue::LinkTextToStop(const std::string& in) {
	std::unordered_set<Stop>::iterator It = stops_.find(in);
	if (It == stops_.end())
		throw std::logic_error("Non-existent Stop in Route");
	const transport_catalogue::Stop* cpt = &(*It);
	transport_catalogue::Stop* pt =
		const_cast<transport_catalogue::Stop*>(cpt);
	return pt;
}

transport_catalogue::Route transport_catalogue::BasicParse(
	const std::string& raw_desc, const char delim) {

	size_t j = raw_desc.size();
	Route ret;
	ret.way_.reserve(j / 5);
	size_t bgn = 0, end = 0, sz = 0;

	for (size_t i = 0; i < j; ++i) {
		if (raw_desc[i] == delim || i == j - 1) {
			//++ret.unique_stops_;
			end = (i < j - 1) ? i : j + 1;
			sz = end - bgn - 1;
			ret.way_.push_back(LinkTextToStop(raw_desc.substr(bgn, sz)));
			bgn = end + 2;
		}
	}

	return ret;
}

transport_catalogue::Route transport_catalogue::ParseLinearRoute(
	const std::string& raw_desc) {

	Route ret = BasicParse(raw_desc, BusLineDelimeter);

	// adding tail
	ret.way_.reserve(ret.way_.size() * 2);
	ret.way_.insert(ret.way_.end(), ret.way_.rbegin() + 1, ret.way_.rend());
	ret.way_.shrink_to_fit();

	ret.unique_stops_ = CalcUniques(ret);
	CalcDistances(ret);
	return ret;
}

transport_catalogue::Route transport_catalogue::ParseCircularRoute(
	const std::string& raw_desc) {

	Route ret = BasicParse(raw_desc, BusCircleDelimeter);

	ret.way_.shrink_to_fit();
	ret.unique_stops_ = CalcUniques(ret);
	CalcDistances(ret);
	return ret;
}

const transport_catalogue::Route& transport_catalogue::GetRoute(const std::string& BusName) {
	std::unordered_map<std::string, transport_catalogue::Route>::iterator It
		= routes_.find(BusName);
	
	return It != routes_.end() ? It->second : empty_;
}

size_t transport_catalogue::CalcUniques(const Route& route) {
	std::set<Stop*> each;
	for (const Stop* stop : route.way_) {
		const Stop* tmp1 = stop;
		Stop* tmp = const_cast<Stop*>(tmp1);
		each.insert(tmp);
	}
	return each.size();
}

void transport_catalogue::CalcDistances(transport_catalogue::Route& route) {
	for (size_t i = 1, j = route.way_.size(); i < j; ++i) {
		route.length_ += DistBetween(route.way_[i - 1], route.way_[i]);
	}
}

double transport_catalogue::DistBetween(
	transport_catalogue::Stop*& a, 
	transport_catalogue::Stop*& b) {

	if (distances_.count(a) == 0 || distances_.at(a).count(b) == 0) {
		double dist = geo::ComputeDistance(a->coords_, b->coords_);
		distances_[a][b] = dist;
		distances_[b][a] = dist;
	}

	return distances_.at(a).at(b);
}

bool transport_catalogue::Stop::operator<(const Stop& other) const {
	return name_ < other.name_;
}

bool transport_catalogue::Stop::operator==(const Stop& other) const {
	return name_ == other.name_;
}

bool transport_catalogue::Bus::operator<(const Bus& other) const {
	return name_ < other.name_;
}

bool transport_catalogue::Bus::operator==(const Bus& other) const {
	return name_ == other.name_;
}
