#pragma once

#include <ctime>
struct time_counter_t
{
private:
	clock_t start_time_ = 0;
	clock_t count_ = 0;
public:
	void start()
	{
		this->start_time_ = ::clock();
	}
	clock_t end()
	{
		this->count_ = current();
		return this->count_;
	}
	clock_t current() const
	{
		return ::clock() - this->start_time_;
	}

	clock_t time() const { return this->count_; }
};