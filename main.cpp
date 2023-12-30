#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Candlestick {
    float open, high, low, close;
};

void generateCandlestickChart(const Candlestick* data, int dataSize, int height = 50, int range = 200) {
    // Implementacja generowania wykresu świecowego
    // ...

    // Poniżej znajduje się przykład wyświetlenia wykresu w konsoli
    for (int i = 0; i < range; ++i) {
        for (int j = 0; j < dataSize; ++j) {
            char bodyChar = (data[j].open < data[j].close) ? 'O' : '#'; // Wybór znaku ciała świecy
            cout << bodyChar;
        }
        cout << endl;
    }
}

void loadDataFromFile(Candlestick*& data, int& dataSize) {
    fstream file;
    file.open("intc_us_data.csv", ios::in);
    bool isOpen = file.is_open();
    if (!isOpen)
    {
        cout << "Błąd otwarcia pliku." << endl;
        return;
    }

    // Wczytaj dane z pliku CSV
    string line;
    getline(file, line); // Pomijamy nagłówek

    dataSize = 0;
    while (getline(file, line)) {
        dataSize++;
    }

    file.clear();
    file.seekg(0, ios::beg); // Przewiń na początek pliku

    data = new Candlestick[dataSize];

    int index = 0;
    while (getline(file, line)) {
        Candlestick& candle = data[index];
        stringstream ss(line);
        string value;

        getline(ss, value, ',');
        candle.open = stof(value);

        getline(ss, value, ',');
        candle.high = stof(value);

        getline(ss, value, ',');
        candle.low = stof(value);

        getline(ss, value, ',');
        candle.close = stof(value);

        index++;
    }

    file.close();
}

void cleanupData(Candlestick*& data) {
    delete[] data;
    data = nullptr;
}

int main() {
    Candlestick* stockData = nullptr;
    int dataSize = 10000;
    loadDataFromFile(stockData, dataSize);

    // Wywołaj funkcję generującą wykres
    generateCandlestickChart(stockData, dataSize);

    // Zwolnij pamięć
    cleanupData(stockData);

    return 0;
}
