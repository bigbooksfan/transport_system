#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <string_view>
#include <functional>

#include "geo.h"

class transport_catalogue {
private:			// structs
	struct Route {
		std::vector<std::string_view> way_;
		size_t unique_stops_ = 0;
		double length = 0;
	};

	struct Stop {
		std::string name_;
		Coordinates coords_;

		// constructor for searching by the name with calling stops_.count(name)
		Stop(const std::string& name = "", const Coordinates& coords = Coordinates{})
			: name_(name), coords_(coords) { };

		//Stop(const Stop&) = default;
		//Stop(Stop&&) = default;
		//Stop& operator=(const Stop&) = default;
		//~Stop() = default;

		bool operator<(const Stop& other) const;
		bool operator==(const Stop& other) const;
	};

	struct StopHasher {
		size_t operator()(const Stop& stop) const {
			return static_cast<size_t>(
				std::hash<std::string>{}(stop.name_)
				//static_cast<size_t>(stop.coords_.lat) * 97 +
				//static_cast<size_t>(stop.coords_.lng) * 97 * 97
				);
		}
	};

private:			// fields
	// HINT : u_map < stop name , coords >
	//std::unordered_map<std::string, Coordinates> stops_;
	// HINT : u_set < struct { string name , Coords } >
	std::unordered_set<Stop, StopHasher> stops_;
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

	void CalcDistances(Route& route);
};

