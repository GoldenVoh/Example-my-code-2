#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

const int SIZE = 10;
const char INITIAL_VALUE = '8';
const int START_ROW = 11;

class FileCreator {
public:
    bool createFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "File " << filename << " not created" << endl;
            return false;
        }
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                file << INITIAL_VALUE;
            }
            file << endl;
        }
        file.close();
        return true;
    }
};

class CommandReader {
public:
    string readCommand() {
        string command;
        getline(cin, command);
        return command;
    }
};

class Robot {
    int x, y;
    int prevX, prevY;
    vector<string> errors;
    fstream file;
public:
    Robot() : x(1), y(1), prevX(-1), prevY(-1) {}

    void openFile(const string& filename) {
        file.open(filename, ios::in | ios::out);
    }

    void closeFile() {
        file.close();
    }

    void move(int newX, int newY) {
        if (newX < 1 || newX > SIZE || newY < 1 || newY > SIZE) {
            addError("Coordinate is wrong ( " + to_string(newX) + ", " + to_string(newY) + " )");
            return;
        }
        x = newX;
        y = newY;
    }

    void write(char symbol) {
        if (!isalpha(symbol)) {
            addError("The letter does not belong to the Latin alphabet: " + string(1, symbol));
            return;
        }

        if (x == prevX && y == prevY) {
            addError("Attempt to sequentially write to a cell ( " + to_string(x) + ", " + to_string(y) + " )");
            return;
        }

        file.seekp((x - 1) * (SIZE + 2) + (y - 1));
        file.put(symbol);

        prevX = x;
        prevY = y;
    }

    void addError(const string& error) {
        errors.push_back(error);
    }

    void logErrors() {
        file.seekp(START_ROW * (SIZE + 1), ios::beg);
        for (const string& error : errors) {
            file << error << endl;
        }
    }

};

class System {
    FileCreator fileCreator;
    CommandReader commandReader;
    Robot robot;
    string filename;

public:
    void buildTreeObjects() {

    }

    void execApp() {
        filename = "field.txt";
        if (!fileCreator.createFile(filename)) {
            return;
        }
        robot.openFile(filename);

        while (true) {
            string command = commandReader.readCommand();
            if (command == "END") {
                break;
            }
            processCommand(command);
        }

        robot.logErrors();
        robot.closeFile();
        displayFile();
    }

    void processCommand(const string& command) {
        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "MOVE") {
            int x, y;
            ss >> x >> y;
            robot.move(x, y);
        }
        else if (cmd == "WRITE") {
            char symbol;
            ss >> symbol;
            robot.write(symbol);
        }
        else if (cmd == "SHOWTREE") {
            showTree();
        }
        else {
            robot.addError("ERROR command: " + command);
        }
    }

    void displayFile() {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    void showTree() {
        cout << "Объект система";
        cout << "    Объект создания и подготовки файла ";
        cout << "    Объект для считывания команд";
        cout << "    Объект робот";
        cout << "        Объект перемещения робота";
        cout << "        Объект для записи символа в файл";
        cout << "        Объект для записи ошибок";
        cout << "        Объект для вывода ошибок";
        cout << "    Объект для вывода";
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    System system;
    system.buildTreeObjects();
    system.execApp();
    return 0;
}