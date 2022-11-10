#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex kitchen;
std::mutex courier;
std::mutex oper;
std::vector<std::string> queue; //очередь на готовку
std::vector<std::string> ready; //очередь готовых блюд
int counter = 0; //счетчик доставок

void cooking() {
	std::string current; //текущее блюдо на кухне
	
	do {
		if (!queue.empty()) {
			kitchen.lock();
			current = queue[0];
			queue.erase(queue.begin());
			std::cout << "Now cooking: " << current << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(15 - rand() % 10));
			std::cout << "Order ready: " << current << std::endl;
			ready.push_back(current);
			kitchen.unlock();
		}
	} while (counter < 29);
}

void order(std::vector<std::string> menu) {
	int dish; //блюдо заказа
	do {
		std::this_thread::sleep_for(std::chrono::seconds(10 - rand() % 5));
		dish = rand() % (menu.size() - 1);
		std::cout << "New order: " << menu[dish] << std::endl;
		oper.lock();
		queue.push_back(menu[dish]);
		oper.unlock();
	} while (counter < 29);
}

void delivery() {
	do {
		std::this_thread::sleep_for(std::chrono::seconds(30));
		std::cout << counter+1 << " Delivery: " << std::endl;
		
		do {
			std::cout << ready[0] << std::endl;
			courier.lock();
			ready.erase(ready.begin());
			courier.unlock();
		} while (ready.size() > 0);
		
		++counter;
	} while (counter < 30);
}

int main() {
	std::vector <std::string> menu = { "pizza", "soup", "steak", "salad", "sushi"};

	std::thread makeOrder(order, menu);
	std::thread cook(cooking);
	std::thread deliver(delivery);

	makeOrder.join();
	cook.detach();
	deliver.join();

	return 0;
}