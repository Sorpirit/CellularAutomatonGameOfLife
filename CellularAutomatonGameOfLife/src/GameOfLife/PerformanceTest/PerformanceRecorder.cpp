#include "PerformanceRecorder.hpp"

#include <ranges>


namespace PerformanceTest
{
	Stat::Stat(const string statName) :
	Name(statName)
	{
		statFile = ofstream(statName + "_log.csv");
		statFile << statName + ",\n";
	}

	Stat::~Stat()
	{
		statFile.close();
	}

	void Stat::Write(long nanoseconds)
	{
		statFile << to_string(nanoseconds) + ",\n";
	}

	PerformanceRecorder::~PerformanceRecorder()
	{
		for (const auto val : _map | views::values)
		{
			delete val;
		}
		_map.clear();
	}

	void PerformanceRecorder::begin(const std::string statName)
	{
		Stat* stat;
		if (_map.contains(statName))
			stat = _map[statName];
		else
		{
			stat = new Stat(statName);
			_map[statName] = stat;
		}

		stat->LastStart = high_resolution_clock::now();
	}

	void PerformanceRecorder::end(const std::string statName)
	{
		auto currentTime = high_resolution_clock::now();

		if (!_map.contains(statName))
			return;

		Stat* stat = _map[statName];
		auto duration = currentTime - stat->LastStart;
		auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
		long nanosecondsDuration = value.count();
		stat->Write(nanosecondsDuration);
	}
}
