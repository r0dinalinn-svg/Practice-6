#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

enum TrafficLightsState {
    RED,
    YELLOW,
    GREEN
};

struct Address {
    string country;
    string city;
    string street;
    string coordinates;
};

class TrafficLight {
private:
    Address address;

public:
    TrafficLight(Address addr) : address(addr) {}

    TrafficLightsState GetCurrentState() {
        auto now = chrono::system_clock::now();
        auto now_sec = chrono::floor<chrono::seconds>(now.time_since_epoch());
        int seconds = (now_sec % chrono::seconds(60)).count();

        if (seconds >= 0 && seconds <= 5) {
            return YELLOW;
        }
        else if (seconds >= 6 && seconds <= 25) {
            return RED;
        }
        else {
            return GREEN;
        }
    }

    string GetColorName(TrafficLightsState state) {
        switch (state) {
        case RED: return "Красный";
        case YELLOW: return "Желтый";
        case GREEN: return "Зеленый";
        default: return "Неизвестно";
        }
    }

    void GetInfo() {
        TrafficLightsState currentState = GetCurrentState();
        cout << "--- Информация о светофоре ---" << endl;
        cout << "Адрес: " << address.country << ", " << address.city << ", "
            << address.street << " (" << address.coordinates << ")" << endl;
        cout << "Текущий свет: " << GetColorName(currentState) << endl;
        cout << "-----------------------------" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Address testAddress = { "Россия", "Москва", "Тверская улица", "55.7558, 37.6173" };
    TrafficLight myLight(testAddress);

    while (true) {
        myLight.GetInfo();
        this_thread::sleep_for(chrono::milliseconds(23000));
    }

    return 0;
}