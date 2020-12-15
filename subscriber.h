#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include <thread>
#include <mutex>



struct ProcessSubscribers {
	virtual void Process(std::vector<std::shared_ptr<Figure>> &buffer) = 0;
	virtual ~ProcessSubscribers() = default;
};

struct ProcessConsole : ProcessSubscribers {
	void Process(std::vector<std::shared_ptr<Figure>> &buffer) override {
		for (const auto figure : buffer) {
			figure->Print(std::cout);
		}
	}
};

struct ProcessFile : ProcessSubscribers {
	void Process(std::vector<std::shared_ptr<Figure>> &buffer) override {
		std::ofstream os(std::to_string(this->name));
		for (const auto figure : buffer) {
			figure->Print(os);
		}
		++this->name;
	}

private:
	uint16_t name = 0;
};

struct Subscriber {
	void operator()() {
		for(;;) {
			std::unique_lock<std::mutex> guard(mtx);
			cv.wait(guard, [&](){
				return buffer.size() == buffer.capacity() || end;
			});

			if (end)
				break;

			for (uint16_t i = 0; i < processes.size(); ++i)
				processes[i]->Process(buffer);

			buffer.clear();
			cv.notify_all();
		}
	}

	bool end = false;
	std::vector<std::shared_ptr<Figure>> buffer;
	std::vector<std::shared_ptr<ProcessSubscribers>> processes;
	std::condition_variable cv;
	std::mutex mtx;
};