#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>

using namespace std;

struct Candlestick {
    double open, close, high, low, volume;
    int year, month, day, wick, top, bottom, shadow, index;
};

void menu(string& fileName, string& graphName, int& dataHeight, char& letter);
void loadDataFromFile(Candlestick*& data, int& dataSize, string fileName, int dataHeight, char letter);
void generateCandlestickChart(const Candlestick* data, int dataSize, string graphName, int dataHeight, int range = 200);
void showgraph(const Candlestick* data, int dataSize, int dataHeight, int range = 200);
void cleanupData(Candlestick*& data);

int main() {
    Candlestick* stockData = nullptr;
    int dataSize = 10000;
    string fileName = "intc_us_data.csv";
    string graphName = "graph.txt";
    int dataHeight = 50;
    int range;
    char letter;

    while(letter != 'q' || letter != 'Q')
    {
        menu(fileName, graphName, dataHeight, letter);
        loadDataFromFile(stockData, dataSize, fileName, dataHeight, letter);

        // Wywo�aj funkcj� generuj�c� wykres
        generateCandlestickChart(stockData, dataSize, graphName, dataHeight);



        range = 200;
        if(letter == 't')
        {
            showgraph(stockData, dataSize, dataHeight, range);
        }
        cout << "Naci�nij Enter, aby kontynuowa�...";
        cin.ignore();
        cin.get();
        system("cls");

        // Zwolnij pami��
        cleanupData(stockData);


    }


    return 0;




}



void menu(string& fileName, string& graphName, int& dataHeight, char& letter) {
    cout << "-----------------------------marketGraph-----------------------------\n\n\n\n\n";
    cout << "If you want to see default graph \"intc_us_data.csv\" type: \'G\' or \'g\'\n";
    cout << "Type file name: ";    cin >> fileName;
    if(fileName == "g" || fileName == "G")
    {
        letter = 'g';
        fileName = "intc_us_data.csv";
        graphName = "graph.txt";
        cout << "Type graph height [default = 50]: "; cin >> dataHeight;
    }
    else if(fileName == "t" || fileName == "T")
    {
        letter = 't';
        fileName = "intc_us_data.csv";
        graphName = "graph.txt";
    }
    else if(fileName == "q" || fileName == "Q")
    {
        letter = 'q';
    }
    else
    {
        cout << "Type graph (.txt) file name: ";    cin >> graphName;
    }

}


void loadDataFromFile(Candlestick*& data, int& dataSize, string fileName, int dataHeight, char letter) {
    fstream file;
    file.open(fileName, ios::in);

    bool isOpen = file.is_open();

    if (!isOpen && letter !='q') {
        cout << "File opening error!" << endl;
        return;
    }

    string line;
    dataSize = 0;
    // Wczytaj dane z pliku CSV
    getline(file, line); // Pomijamy nag��wek

    while (getline(file, line))
    {
        dataSize++;
    }

    file.clear();
    file.seekg(0, ios::beg); // Przewi� na pocz�tek pliku

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
        data[index].open = stod(buf) * (dataHeight / 50);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].high = stod(buf) * (dataHeight / 50);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].low = stod(buf) * (dataHeight / 50.);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].close = stod(buf) * (dataHeight / 50.);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].volume = stod(buf);

        data[index].index = index;
        data[index].wick = round(data[index].high);
        data[index].top = round(data[index].open);
        data[index].bottom = round(data[index].close);
        data[index].shadow = round(data[index].low);



        index++;
    }

    file.close();
}

void generateCandlestickChart(const Candlestick* data, int dataSize, string graphName, int dataHeight, int range) {
    char letter;
    ofstream file;
    file.open(graphName);

    for (int i = 0; i < dataHeight; ++i)
    {
        for (int j = dataSize - range; j + 1 < dataSize; ++j)
        {
            int actualValue = dataHeight - (i + 1);

            if (data[j].top - data[j].bottom > 0)
                letter = '#';
            else
                letter = '0';

            if ((actualValue  >= data[j].top && actualValue  <= data[j].bottom) || (actualValue  <= data[j].top && actualValue  >= data[j].bottom))
            {
                file << letter;
            }else if((actualValue > data[j].top && actualValue <= data[j].wick) || (data[j].wick <= actualValue && data[j].bottom > actualValue) || (data[j].top > actualValue && data[j].shadow <= actualValue) || (data[j].bottom > actualValue && data[j].shadow <= actualValue))
            {
                file << '|';
            }else
            {
                file << ' ';
            }
        }
        file << endl;

    }
}

void showgraph(const Candlestick* data, int dataSize, int dataHeight, int range)
{

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    char letter;

    for (int i = 0; i < dataHeight; ++i)
    {
        for (int j = dataSize - range; j + 1 < dataSize; ++j)
        {
            int actualValue = dataHeight - (i + 1);

            if (data[j].top - data[j].bottom > 0)
                letter = '#';
            else
                letter = '0';

            if ((actualValue  >= data[j].top && actualValue  <= data[j].bottom) || (actualValue  <= data[j].top && actualValue  >= data[j].bottom))
            {
                cout << letter;
            }else if((actualValue > data[j].top && actualValue <= data[j].wick) || (data[j].wick <= actualValue && data[j].bottom > actualValue) || (data[j].top > actualValue && data[j].shadow <= actualValue) || (data[j].bottom > actualValue && data[j].shadow <= actualValue))
            {
                cout << '|';
            }else
            {
                cout << ' ';
            }
        }
        cout << endl;

    }

}

void cleanupData(Candlestick*& data) {
    delete[] data;
    data = nullptr;
}
