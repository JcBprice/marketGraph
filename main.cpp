#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <iomanip>

using namespace std;

struct Candlestick {
    double open, close, high, low, volume;
    int year, month, day, wick, top, bottom, shadow, index;
};
void clear();
void logFile(const string& action, const string& words);
void menu(string& fileName, string& graphName, int& dataHeight, char& letter, bool& firstLoop, unsigned int& counter, bool & isType);
void loadDataFromFile(Candlestick*& data, int& dataSize, string fileName, int dataHeight, char letter, int& h_w, int& h_oc, int& l_s, int& l_oc);
void generateCandlestickChart(const Candlestick* data, int dataSize, string graphName, int dataHeight, int& h_w, int& h_oc, int& l_s, int& l_oc, int range = 200);
void showgraph(const Candlestick* data, int dataSize, int dataHeight, int& h_w, int& h_oc, int& l_s, int& l_oc);
void cleanupData(Candlestick*& data);

int main() {
    Candlestick* stockData = nullptr;
    int range, dataSize = 10000, dataHeight = 50, h_w = 0, h_oc = 0, l_s = 9999, l_oc = 9999;
    string fileName = "intc_us_data.csv", graphName = "chart.txt";
    bool isType, firstLoop = true;
    char letter;
    unsigned int counter = 1;

    while(letter != 'q' && letter != 'Q')
    {
        h_w = 0, h_oc = 0, l_s = 99999, l_oc = 99999;

        menu(fileName, graphName, dataHeight, letter, firstLoop, counter, isType);
        loadDataFromFile(stockData, dataSize, fileName, dataHeight, letter, h_w, h_oc, l_s, l_oc);


        if(letter != 'q' && letter != 'Q')
        {
            generateCandlestickChart(stockData, dataSize, graphName, dataHeight, h_w, h_oc, l_s, l_oc);



            if(letter == 't')
            {
                showgraph(stockData, dataSize, dataHeight, h_w, h_oc, l_s, l_oc);
            }
            cout << "Press enter to continue...";
            cin.ignore();
            cin.get();
            clear();

            // Zwolnij pamięć
            cleanupData(stockData);

        }

    }


    return 0;


}

void clear()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void logFile(const string& action, const string& words)
{
    ofstream logFile("user_actions.log", ios::app);
    if (logFile.is_open()) {
        time_t now = time(NULL);
        tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", timeinfo);
        if(action != "0")
            logFile << buffer << action << endl;
        if(words != "0")
            logFile << buffer << words << endl;
        logFile.close();

    } else {
        cout << "Error opening log file." << endl;
    }

}

void menu(string& fileName, string& graphName, int& dataHeight, char& letter, bool& firstLoop, unsigned int& counter, bool& isType) {

    if(!isType || letter == 'u')
    {
        cout << "-----------------------------marketGraph-----------------------------\n\n\n\n\n";
        cout << "To see the default chart \"intc_us_data.csv\" type: \"G\" or \"g\"\n";

        while(fileName != "q" && fileName != "Q")
        {
            cout << "Type .csv file name: ";
            cin >> fileName;
            logFile("0","User typed file name: " + fileName);

            if (fileName == "g" || fileName == "g")
                break;

            if ((fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".csv"))
                break;
            else
                cout << "Incorrect file name. Correct name: [your_file].csv\n" << endl;

        }

        while (fileName != "q" && fileName != "Q" && fileName != "g" && fileName != "G")
        {
            cout << "Type .txt file name: ";
            cin >> graphName;
            logFile("0","User typed graph name: " + graphName);
            counter = 0;

            if (graphName.size() > 4 && graphName.substr(graphName.size() - 4) == ".txt" )
                break;
            else
                cout << "Incorrect file name. Correct name: [your_file].txt\n" << endl;
        }

        if(counter == 0)
        {
            cout << "Type chart height [default = 50]: ";     cin >> dataHeight;
            counter++;
            string buf;
            ostringstream oss;
            oss << dataHeight;
            logFile("0", "User typed data height: " + oss.str());
        }
        isType = true;

    }

    else if(letter != 'q' && letter != 'u')
    {
        clear();
        cout << "Instructions:\n\n";
        cout << "Type \'t\' or \'T\' to generate 200 most recent resulsts.\n";
        cout << "Type \'u\' or \'U\' to load different chart.\n";
        cout << "Type \'q\' or \'Q\' to close the program.\n\n\n";
        cout << "Actual file: \"" << fileName << "\".\n";
        cout << "The chart \"" << graphName << "\" has been generated.\n\n";

        do{
            cout << "Type: "; cin >> letter;
            logFile("User typed letter: " + string(1, letter), "0");
        }while(letter != 't' && letter != 'q' && letter != 'u' && letter != 'T' && letter != 'Q' && letter != 'U');

    }
    if(fileName == "g" || fileName == "G" || letter == 'G')
    {
        letter = 'g';
        fileName = "intc_us_data.csv";
        graphName = "chart.txt";
        cout << "Type chart height [default = 50]: "; cin >> dataHeight;
    }
    if(fileName == "q" || fileName == "Q" || letter == 'Q')
        letter = 'q';

    if(letter == 'u') {
        letter = '\0';
        isType = false;
    }

    if(fileName == "u" || fileName == "U" || letter == 'U')
    {
        letter = 'u';
        counter = 0;
        clear();
    }

    firstLoop = false;


}


void loadDataFromFile(Candlestick*& data, int& dataSize, string fileName, int dataHeight, char letter, int& h_w, int& h_oc, int& l_s, int& l_oc)
{
    int range = 200;
    fstream file;
    file.open(fileName, ios::in);

    bool isOpen = file.is_open();

    if (!isOpen && letter !='q')
    {
        cout << "File opening error!" << endl;
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
        data[index].open = stod(buf) * (static_cast<double>(dataHeight) / 50.);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].high = stod(buf) *  (static_cast<double>(dataHeight) / 50.);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].low = stod(buf) *  (static_cast<double>(dataHeight) / 50.);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].close = stod(buf) *  (static_cast<double>(dataHeight) / 50.);

        getline(ss, value, ',');
        strncpy(buf, value.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        data[index].volume = stod(buf);


        data[index].index = index;
        data[index].wick = round(data[index].high);
        data[index].top = round(data[index].open);
        data[index].bottom = round(data[index].close);
        data[index].shadow = round(data[index].low);


        if(index >= dataSize - range)
        {


            if(h_oc < data[index].bottom)
                h_oc = data[index].bottom;
            if(h_oc < data[index].top)
                h_oc = data[index].top;

            if(h_w < data[index].wick)
                h_w = data[index].wick;

            if(l_s > data[index].shadow)
                l_s = data[index].shadow;

            if(l_oc > data[index].top)
                l_oc = data[index].top;
            if(l_oc > data[index].bottom)
                l_oc = data[index].bottom;






        }


        index++;
    }

    file.close();
}

void generateCandlestickChart(const Candlestick* data, int dataSize, string graphName, int dataHeight, int& h_w, int& h_oc, int& l_s, int& l_oc, int range) {
    char letter;
    ofstream file;
    file.open(graphName);

    clear();

    for (int i = -1; i <= dataHeight - 1; ++i)
    {
        int actualValue = dataHeight - i;

        if (actualValue == h_w || actualValue  == h_oc || actualValue == l_oc || actualValue  == l_s || actualValue == 0)
            file << setw(7) << 50. /dataHeight * actualValue;
        else if (i == -1)
            file << setw(7) << "Value\n";
        else
            file << setw(7) << "|";
        for (int j = dataSize - range - 1; j < dataSize && i > -1; ++j)
        {

            if (data[j].top - data[j].bottom > 0)
                letter = '#';
            else
                letter = '0';

            if ((actualValue >= data[j].top && actualValue <= data[j].bottom) || (actualValue <= data[j].top && actualValue >= data[j].bottom))
            {
                file << letter;
            }
            else if ((actualValue > data[j].top && actualValue <= data[j].wick) || (data[j].wick <= actualValue && data[j].bottom > actualValue) || (data[j].top > actualValue && data[j].shadow <= actualValue) || (data[j].bottom > actualValue && data[j].shadow <= actualValue))
            {
                file << '|';
            }
            else
            {
                file << ' ';
            }
        }

        if (i == dataHeight - 1)
        {
            file << endl << setw(7) << "#";
            for (int j = 1; j <= range; j++)
                file << "-";
        }
        file << endl;
    }

    file << setw(8) << data[dataSize - range - 1].day << "." << data[dataSize - range - 1].month << "." << data[dataSize - range - 1].year;
    for (int i = 1; i <= range - 17; i++)
        file << " ";
    file << data[dataSize - 1].day << "." << data[dataSize - 1].month << "." << data[dataSize - 1].year << endl;

    for (int i = 1; i <= range - 7; i++)
        file << " ";
    file << "Date [DD/MM/YY]\n\n";
}


void showgraph(const Candlestick* data, int dataSize, int dataHeight, int& h_w, int& h_oc, int& l_s, int& l_oc)
{
    char letter;
    int range = 200;

    clear();

    for (int i = -1; i <= dataHeight - 1; ++i)
    {
        int actualValue = dataHeight - i;

        if (actualValue == h_w || actualValue  == h_oc || actualValue == l_oc || actualValue  == l_s || actualValue == 0)
            cout << setw(7) << 50. /dataHeight * actualValue;
        else if (i == -1)
            cout << setw(7) << "Value\n";
        else
            cout << setw(7) << "|";
        for (int j = dataSize - range - 1; j < dataSize && i > -1; ++j)
        {

            if (data[j].top - data[j].bottom > 0)
                letter = '#';
            else
                letter = '0';

            if ((actualValue >= data[j].top && actualValue <= data[j].bottom) || (actualValue <= data[j].top && actualValue >= data[j].bottom))
            {
                cout << letter;
            }
            else if ((actualValue > data[j].top && actualValue <= data[j].wick) || (data[j].wick <= actualValue && data[j].bottom > actualValue) || (data[j].top > actualValue && data[j].shadow <= actualValue) || (data[j].bottom > actualValue && data[j].shadow <= actualValue))
            {
                cout << '|';
            }
            else
            {
                cout << ' ';
            }
        }

        if (i == dataHeight - 1)
        {
            cout << endl << setw(7) << "#";
            for (int j = 1; j <= range; j++)
                cout << "-";
        }
        cout << endl;
    }

    cout << setw(8) << data[dataSize - range - 1].day << "." << data[dataSize - range - 1].month << "." << data[dataSize - range - 1].year;
    for (int i = 1; i <= range - 17; i++)
        cout << " ";
    cout << data[dataSize - 1].day << "." << data[dataSize - 1].month << "." << data[dataSize - 1].year << endl;

    for (int i = 1; i <= range - 7; i++)
        cout << " ";
    cout << "Date [DD/MM/YY]\n\n";

}

void cleanupData(Candlestick*& data)
{
    delete[] data;
    data = nullptr;
}
