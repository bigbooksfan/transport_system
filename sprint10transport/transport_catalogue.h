#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <string_view>
#include <functional>

#include "geo.h"

class transport_catalogue {
public:			// structs
	struct Stop {
		std::string name_;
		geo::Coordinates coords_;

		// constructor for searching by the name with calling stops_.count(name)
		Stop(const std::string& name = "", const geo::Coordinates& coords = geo::Coordinates{})
			: name_(name), coords_(coords) { };

		//Stop(const Stop&) = default;
		//Stop(Stop&&) = default;
		//Stop& operator=(const Stop&) = default;
		//~Stop() = default;

		bool operator<(const Stop& other) const;
		bool operator==(const Stop& other) const;
	};

	struct Bus {
		std::string name_;
		std::string raw_route_;

		Bus(const std::string& name = "", const std::string& raw_route = "")
			: name_(name), raw_route_(raw_route) { }

		bool operator<(const Bus& other) const;
		bool operator==(const Bus& other) const;
	};

	// HINT : struct { vector<sv> stops , unique_stops, length }
	struct Route {
		//std::vector<std::string_view> way_;
		std::vector<Stop*> way_;
		size_t unique_stops_ = 0;
		double length_ = 0;
	};

	// T = struct Bus or struct Stop
	template <typename T>
	struct StringHasher {
		size_t operator()(const T& stop) const {
			return std::hash<std::string>{}(stop.name_);
		}
	};

private:			// fields
	// HINT : u_set < struct Stop { string name , Coords } >
	std::unordered_set<Stop, StringHasher<Stop>> stops_;
	// HINT : u_set < struct Bus { name , raw route } >
	std::unordered_set<Bus, StringHasher<Bus>> buses_;
	// HINT : u_map < bus name , vector < string_view Stops> >
	std::unordered_map<std::string, Route> routes_;
	Route dummy_;
	Route empty_;

	const char BusLineDelimeter = '-';
	const char BusCircleDelimeter = '>';

	// HINT : map < Stop1* , map < Stop2* , distance > >
	std::map<Stop*, std::map<Stop*, double>> distances_;

public:				// constructors
	transport_catalogue(std::string& number);
	transport_catalogue(size_t number);

public:				// methods
	void AddBus(const std::string& name, const std::string& raw_desc);
	void AddStop(const std::string& name, const double lat, const double lon);
		
	// Route information out
	const Route& GetRoute(const std::string& BusName);

	const Stop& FindStop(const std::string& name) const;
	const Bus& FindBus(const std::string& name) const;

private:			// methods
	void ParseRoutes();
	Stop* LinkTextToStop(const std::string& in);
	Route BasicParse(const std::string& raw_desc, const char delim);
	Route ParseLinearRoute(const std::string& raw_desc);
	Route ParseCircularRoute(const std::string& raw_desc);
	
	size_t CalcUniques(const Route& route);
	void CalcDistances(Route& route);
	double DistBetween(Stop*& a, Stop*& b);
};

