#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <ctime>
#include <conio.h>

using namespace std;

vector<string> merch;
vector<vector<int>> matrix;
map<int, int> myPinMap;
int mno = 1, pno = 0;

void addmerch() {
    static int lineCounter = 0;
    ++mno;
    ifstream pinFile("./pin_codes.txt");
    if (!pinFile) {
        cerr << "Error opening pin_codes.txt" << endl;
        return;
    }

    for (int i = 0; i < lineCounter; ++i) {
        string line;
        if (!getline(pinFile, line)) {
            cerr << "Error: Not enough lines in the pin_codes.txt file" << endl;
            pinFile.close();
            return;
        }
    }

    vector<int> pinCodes;
    int pin;
    for (int i = 0; i < 150; ++i) {
        if (pinFile >> pin) {
            pinCodes.push_back(pin);
        } else {
            cerr << "Error reading pin codes from file" << endl;
            return;
        }
    }

    pinFile.close();

    char buf[20];
    sprintf(buf, "merch%d", mno - 1);
    merch.push_back(buf);
    matrix.resize(mno);
    matrix[mno - 1].resize(pno);

    for (int i = 0; i < pinCodes.size(); ++i) {
        int pincode = pinCodes[i];
        auto it = myPinMap.find(pincode);
        if (it != myPinMap.end()) {
            matrix[mno - 1][it->second] = 1;
        } else {
            myPinMap.insert({pincode, pno});
            pno++;
            matrix[mno - 1].push_back(1);
        }
    }
    ++lineCounter;
    if (lineCounter == 150) {
        lineCounter = 0; 
    }
}




void searchmerch() {
    int val;
    printf("Enter pin code to find merchants that deliver: ");
    scanf("%d", &val);

    auto it = myPinMap.find(val);
    if (it == myPinMap.end()) {
        printf("Pin code %d not available\n", val);
        return;
    }

    int key = it->second;
    int j = 0;
    for (int i = 1; i < mno; i++) {
        if (matrix[i][key] == 1) {
            printf("%s\n", merch[i].c_str());
            ++j;
        }
    }
    printf("%d\n", j);
}

int main() {
    int y;
    srand(time(0));

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int m = 0; m < 10000; m++) {
        addmerch();
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to insert merchants and deliveries: %lf seconds\n", cpu_time_used);

    start = clock();
    searchmerch();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to search for merchants: %lf seconds\n", cpu_time_used);
    getch();
    return 0;
    
}
