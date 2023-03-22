#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

#include "geo.h"

class transport_catalogue {
private:			// structs
	struct Route {
		std::vector<std::string_view> way_;
		size_t unique_stops_ = 1;
		double length = 0;
	};

private:			// fields
	// HINT : u_map < stop name , coords >
	std::unordered_map<std::string, Coordinates> stops_;
	// HINT : u_map < bus name , raw route >
	std::unordered_map<std::string, std::string> buses_;
	// HINT : u_map < bus name , vector < string_view Stops> >
	std::unordered_map<std::string, Route> routes_;

	const char BusLineDelimeter = '-';
	const char BusCircleDelimeter = '>';

public:				// constructors
	transport_catalogue(std::string& number);

public:				// methods
	void AddBus(const std::string& name, const std::string& raw_desc);
	void AddStop(const std::string& name, const Coordinates& coords);
		
	// Route information print in console
	void /**/ GetRoute();

	const std::string& FindBus(const std::string& name) const;
	const Coordinates& FindStop(const std::string& name) const;

private:			// methods
	void ParseRoutes();
	Route ParseLinearRoute(const std::string& raw_desc);
	Route ParseCircularRoute(const std::string& raw_desc);

};

