#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <iomanip>

std::mutex wait;

void setTime(std::time_t time, std::tm* timeLocal, int dif) { //функция расчета времени
	time = std::time(nullptr);
	timeLocal = std::localtime(&time);

	if (timeLocal->tm_sec + dif < 60) {
		timeLocal->tm_sec += dif;
	}
	else {
		timeLocal->tm_sec = dif - (60 - timeLocal->tm_sec);
		if (timeLocal->tm_min + dif / 60 < 60) {
			timeLocal->tm_min += 1;
		}
		else {
			timeLocal->tm_min = dif / 60;
			if (timeLocal->tm_hour + dif / 3600 < 24) {
				timeLocal->tm_hour += dif / 3600;
			}
			else {
				timeLocal->tm_hour = 0;
			}
		}
	}
}

void train(std::string name, std::time_t time, std::tm* timeLocal) {
	wait.lock();

	std::string command;
	int timeOnWay;
	time = std::time(nullptr);
	timeLocal = std::localtime(&time);

	std::cout << "Time now: " << std::put_time(timeLocal, "%H:%M:%S") << std::endl; //Текущее время

	setTime(time, timeLocal, 15);

	std::cout << "Train " << name << " will depart at " << std::put_time(timeLocal, "%H:%M:%S") << std::endl; //Время отправления поезда со станции

	std::this_thread::sleep_for(std::chrono::seconds(15)); //Ожидание отправления

	time = std::time(nullptr);
	timeLocal = std::localtime(&time);

	std::cout << "Time now: " << std::put_time(timeLocal, "%H:%M:%S") << std::endl; //Текущее время
	std::cout << "Train  " << name << " is depart to station" << std::endl; //Оповещение об отправлении со станции
	std::cout << "Enter time on the way in seconds: "; //Ввод времени в пути
	std::cin >> timeOnWay;

	setTime(time, timeLocal, timeOnWay);

	std::cout << "Train " << name << " will arrive on the station at " << std::put_time(timeLocal, "%H:%M:%S") << std::endl; //Вывод времени прибытия на станцию

	wait.unlock();
	std::this_thread::sleep_for(std::chrono::seconds(timeOnWay)); //ожидание прибытия на станцию
	wait.lock();

	std::cout << "Train " << name << " has arrived at station." << std::endl; //Оповещение о прибытии

	while (command != "depart") { //Ввод команды на отправление с конечной станции
		std::cout << "Enter command: ";
		std::cin >> command;
		if (command == "depart") {
			std::cout << "Train " << name << " has departed." << std::endl;
			
		}
		else {
			std::cout << "Unknown command." << std::endl;
		}
	}

	wait.unlock();
}

int main() {
	std::time_t time = std::time(nullptr);
	std::tm* timeLocal = std::localtime(&time);
	
	wait.lock();
	std::thread train1(train, "A", time, timeLocal);
	wait.unlock();

	wait.lock();
	std::thread train2(train, "B", time, timeLocal);
	wait.unlock();

	wait.lock();
	std::thread train3(train, "C", time, timeLocal);
	wait.unlock();

	train1.join();
	train2.join();
	train3.join();

	return 0;
}