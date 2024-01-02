#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cstdlib>


using namespace std;




struct Candlestick {
    double open, close, high, low, volume;
    int year, month, day, wick, top, bottom, shadow;
};


void loadDataFromFile(Candlestick*& data, int& dataSize, int height = 50);
void generateCandlestickChart(const Candlestick* data, int dataSize, int height = 50, int range = 200);
void cleanupData(Candlestick*& data);




int main()

{
    Candlestick* stockData = nullptr;
    int dataSize = 10000;
    loadDataFromFile(stockData, dataSize);

    // Wywołaj funkcję generującą wykres
    generateCandlestickChart(stockData, dataSize);

    // Zwolnij pamięć
    cleanupData(stockData);

    return 0;
}


void loadDataFromFile(Candlestick*& data, int& dataSize, int height)

{
    fstream file;
    file.open("intc_us_data.csv", ios::in);

    bool isOpen = file.is_open();


    if (!isOpen)

    {
        cout << "Błąd otwarcia pliku." << endl;
        return;
    }

    string line;
    dataSize = 0;
    // Wczytaj dane z pliku CSV
    getline(file, line); // Pomijamy nagłówek


    while (getline(file, line))
    {
        dataSize++;
    }


    file.clear();
    file.seekg(0, ios::beg); // Przewiń na początek pliku


    data = new Candlestick[dataSize + 1];


    int index = 0;
    while (getline(file, line))
    {
        if (line == "Date,Open,High,Low,Close,Volume")
        {
            index++;
            continue;
        }


        string value;
        char buf[20];
        int a;


        istringstream ss(line);

        getline(ss, value, '-');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].year = stoi(buf);

        getline(ss, value, '-');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].month = stoi(buf);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].day = stoi(buf);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].open = stod(buf);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].high = stod(buf);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].low = stod(buf);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].close = stod(buf);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].volume = stod(buf);

        data[index].wick = round(data[index].high);
        data[index].top = round(data[index].open);
        data[index].bottom = round(data[index].close);
        data[index].shadow = round(data[index].low);



        index++;
    }


    file.close();
}


void generateCandlestickChart(const Candlestick* data, int dataSize, int height, int range) {
    char letter;
    ofstream file;

    file.open("graph.txt");


    for (int i = 0; i < height; ++i) {
        for (int j = range; j > 0; --j) {
            bool check = false;
            if (data[j].top - data[j].bottom < 0)
                letter = '#';
            else
                letter = '0';

            if((data[j].shadow == data[j].bottom || data[j].top == data[j].wick) && (data[j].shadow == i || data[j].wick == i))
                file << letter;

            else if ((data[j].wick - data[j].top > 0) && (height - i <= data[j].wick) && (height - i  > data[j].top))
            {
                file << '|';
                !check;
            } else if ((data[j].top - data[j].bottom != 0) && (height - i  <= data[j].top) && (height - i  > data[j].bottom)) {
                file << letter;
                !check;
            } else if ((data[j].bottom - data[j].shadow > 0) && (height - i  >= data[j].shadow) && (height - i  <= round(data[j].bottom))) {
                file << '|';
                !check;
            }

            else {
                file << ' ';
                !check;
            }
        }
        file << endl;


    }
}


void cleanupData(Candlestick*& data) {
    delete[] data;
    data = nullptr;
}