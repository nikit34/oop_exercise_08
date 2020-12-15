#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <cstdlib>

#include "factory.h"
#include "figures.h"
#include "subscriber.h"



int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "need define size" << std::endl;
		return 1;
	}

	uint32_t vector_size = std::atoi(argv[1]);
	Factory factory;

	Subscriber subscriber;
	subscriber.buffer.reserve(vector_size);
	subscriber.processes.push_back(std::make_shared<ProcessConsole>());
	subscriber.processes.push_back(std::make_shared<ProcessFile>());

	std::thread subscriber_thread(std::ref(subscriber));

	std::string cmd;
	std::cout << "add - exit" << std::endl;
	while (std::cin >> cmd) {
		std::unique_lock<std::mutex> main_lock(subscriber.mtx);
		if (cmd == "exit" || cmd == "q" || cmd == "e") {
			subscriber.end = true;
			subscriber.cv.notify_all();
			break;
		} else if (cmd == "add" || cmd == "a") {
			int figure_type;

			for (uint32_t id = 0; id < vector_size; ++id) {
				std::cout << "figure type" << std::endl
				<< "\t1 - rectangle" << std::endl
				<< "\t2 - rhombus" << std::endl
				<< "\t3 - trapezoid" << std::endl;
				std::cin >> figure_type;
				switch (figure_type)
				{
				case 1:{
					std::pair<double, double> *vertices = new std::pair<double, double>[4];
					for (int i = 0; i < 4; ++i)
						std::cin >> vertices[i].first >> vertices[i].second;

					try {
						subscriber.buffer.push_back(factory.FigureCreate(rec, vertices, id));
					} catch (std::logic_error &e) {
						std::cout << e.what() << std::endl;
						--id;
					}
					break;
				}
				case 2: {
					std::pair<double, double> *vertices = new std::pair<double, double>[4];
					for (int i = 0; i < 4; ++i)
						std::cin >> vertices[i].first >> vertices[i].second;

					try {
						subscriber.buffer.push_back(factory.FigureCreate(rhomb, vertices, id));
					} catch (std::logic_error &e) {
						std::cout << e.what() << std::endl;
						id--;
					}
					break;
				}
				case 3: {
					std::pair<double, double> *vertices = new std::pair<double, double>[4];
					for (int i = 0; i < 4; i++)
						std::cin >> vertices[i].first >> vertices[i].second;

					try {
						subscriber.buffer.push_back(factory.FigureCreate(trap, vertices, id));
					} catch (std::logic_error &e) {
						std::cout << e.what() << std::endl;
						id--;
					}
					break;
				}
				default:
					break;
				}
			}

			if (subscriber.buffer.size() == vector_size) {
				//main_lock.unlock();
				subscriber.cv.notify_all();
				subscriber.cv.wait(main_lock, [&subscriber]() {
					return subscriber.success == true;
				});
				subscriber.success = false;
			}
		}
	}
	subscriber_thread.join();
	return 0;
}
