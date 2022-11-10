#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

std::mutex outLock;
std::map <int, std::string> score; //Таблица лидеров

void swimm (std::string name, int speed) {
	int distance = 0; //Текущая пройденная дистанция
	int time; //Время заплыва
	for (time = 0; distance < 100; ++time) {//вывод текущей информации о пловцах
		std::this_thread::sleep_for(std::chrono::seconds(1));
		distance += speed;
		if (distance > 100) distance = 100;
		outLock.lock();
		std::cout << name << ": " << distance << " m" << std::endl;
		std::cout << "----------------" << std::endl;
		outLock.unlock();
	}
	outLock.lock();
	std::cout << name << ": finished" << std::endl; //оповещение о финише пловца
	std::cout << "----------------" << std::endl;
	score.insert(std::pair<int, std::string>(time, name));//Запись результата в таблицу лидеров
	outLock.unlock();
}

int main() {
	std::vector <std::string> names;//вектор имен для одновременной передачи в потоки
	std::vector <int> speeds;//вектор скоростей
	std::string name;
	int speed;
	std::cout << "Enter swimmer's name and speed." << std::endl; //ввод данных пловцов
	for (int i = 0; i < 6; ++i) {
		std::cout << "Swimmer " << i + 1 << std::endl;
		std::cout << "Name: ";
		std::cin >> name;
		std::cout << "Speed: ";
		std::cin >> speed;
		names.push_back(name);
		speeds.push_back(speed);
	}

	std::thread swimmer1(swimm, names[0], speeds[0]);
	std::thread swimmer2(swimm, names[1], speeds[1]);
	std::thread swimmer3(swimm, names[2], speeds[2]);
	std::thread swimmer4(swimm, names[3], speeds[3]);
	std::thread swimmer5(swimm, names[4], speeds[4]);
	std::thread swimmer6(swimm, names[5], speeds[5]);

	swimmer1.join();
	swimmer2.join();
	swimmer3.join();
	swimmer4.join();
	swimmer5.join();
	swimmer6.join();

	for (std::map<int, std::string>::iterator it = score.begin(); it != score.end(); ++it) { //вывод таблицы лидеров на экран
		std::cout << it->second << " - " << it->first << " sec" << std::endl;
	}
	return 0;
}