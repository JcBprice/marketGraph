#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cstdlib>


using namespace std;




struct Candlestick {
    double open, close, high, low, volume;
    int year, month, day, up, down;
};


void loadDataFromFile(Candlestick*& data, int& dataSize);
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


void loadDataFromFile(Candlestick*& data, int& dataSize)

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


        index++;
    }


    file.close();
}


void generateCandlestickChart(const Candlestick* data, int dataSize, int height, int range)
{
    char letter;
    ofstream file;

    file.open("graph.txt");


    for (int i = 0; i < height; ++i)
    {
        for(int j = 1; j < range + 1; ++j)
        {
            if (data[j].open - data[j].close < 0)
                letter = '#';
            else
                letter = 'O';

            if ((round(data[j].high) - round(data[j].open) > 0) && (height - (i + 1) >= round(data[j].high)) && (height - (i + 1) < round(data[j].open)))
                file << '|';
            else if ((round(data[j].open) - round(data[j].close) != 0) && (height - (i + 1) >= round(data[j].open)) && (height - (i + 1) < round(data[j].close)))
                file << letter;
            else if ((round(data[j].close) - round(data[j].low) > 0) && (height - (i + 1) >= round(data[j].close)) && (height - (i + 1) < round(data[j].low)))
                file << '|';
            else
                file << ' ';
        }
        file << endl;
    }
}


void cleanupData(Candlestick*& data) {
    delete[] data;
    data = nullptr;
}