#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>      
#include <nlohmann/json.hpp> 

using namespace std;
using json = nlohmann::json;

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
    int time_offset;

public:
    TrafficLight(Address addr, int offset = 0) : address(addr), time_offset(offset) {}

    TrafficLightsState GetCurrentState() {
        auto now = chrono::system_clock::now();
        auto now_sec = chrono::floor<chrono::seconds>(now.time_since_epoch());
        int seconds = (now_sec % chrono::seconds(60)).count() + time_offset;

        if (seconds % 60 >= 0 && seconds % 60 <= 5) {
            return YELLOW;
        }
        else if (seconds % 60 >= 6 && seconds % 60 <= 25) {
            return RED;
        }
        else {
            return GREEN;
        }
    }

    string GetColorName(TrafficLightsState state) {
        switch (state) {
        case RED: return "красный";
        case YELLOW: return "желтый";
        case GREEN: return "зеленый";
        default: return "неизвестно";
        }
    }

    void GetInfo() {
        TrafficLightsState currentState = GetCurrentState();
        cout << "--- информация о светофоре ---" << endl;
        cout << "адрес: " << address.country << ", " << address.city << ", "
            << address.street << " (" << address.coordinates << ")" << endl;
        cout << "текущий свет: " << GetColorName(currentState) << endl;
        cout << "-----------------------------" << endl;
    }
};

TrafficLight LoadTrafficLightFromFile(const string& filename, int offset) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ошибка: не удалось открыть файл " << filename << endl;
        return TrafficLight({ "неизвестно", "неизвестно", "неизвестно", "неизвестно" }, 0);
    }

    json j;
    file >> j;

    Address addr;
    addr.country = j["country"];
    addr.city = j["city"];
    addr.street = j["street"];
    addr.coordinates = j["coordinates"];

    file.close();
    return TrafficLight(addr, offset);
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    vector<TrafficLight> lights;

    lights.push_back(LoadTrafficLightFromFile("light1.json", 0));
    lights.push_back(LoadTrafficLightFromFile("light2.json", 20));
    lights.push_back(LoadTrafficLightFromFile("light3.json", 40));

    cout << "мониторинг светофоров запущен..." << endl;

    while (true) {
        for (auto& light : lights) {
            light.GetInfo();
        }
        this_thread::sleep_for(chrono::seconds(5));
        system("cls");
    }

    return 0;
}
