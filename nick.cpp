#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include <commdlg.h>

using namespace std;

string OpenFileDialog() {
    char filename[MAX_PATH] = "";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = "csv";

    if (GetOpenFileName(&ofn)) {
        return string(filename);
    }
    else {
        return "";
    }
}

int main() {
    SetConsoleOutputCP(65001);

    cout << "Please click the Open File button." << endl;

    string filename = OpenFileDialog();

    if (filename.empty()) {
        cerr << "No file selected." << endl;
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return 1;
    }

    string line;
    vector<string> nicknames;

    bool isFirstLine = true;

    while (getline(file, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        stringstream ss(line);
        string cell;
        int colIndex = 0;

        while (getline(ss, cell, ',')) {
            if (colIndex == 1) {
                nicknames.push_back(cell);
                break;
            }
            colIndex++;
        }
    }

    file.close();

    ofstream outputFile("nickname_list.txt", ios::out | ios::binary);
    outputFile << "\xEF\xBB\xBF";

    for (size_t i = 0; i < nicknames.size(); ++i) {
        outputFile << nicknames[i];
        if (i != nicknames.size() - 1) {
            outputFile << ", ";
        }
    }

    outputFile << "\nTotal nicknames: " << nicknames.size() << endl;
    outputFile.close();

    cout << "\nThe file 'nickname_list.txt' has been saved." << endl;

    system("pause");
    return 0;
}
