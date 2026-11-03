#include <iostream>
#include <string>
using namespace std;
 

string mod2Divide(string dividend, const string &generator) {
    int n = generator.size();
    string rem = dividend;
 
    for (int i = 0; i <= (int)dividend.size() - n; i++) {
        if (rem[i] == '1') {
            for (int j = 0; j < n; j++)
                rem[i + j] = (rem[i + j] == generator[j]) ? '0' : '1';
        }
    }
    return rem.substr(rem.size() - (n - 1));   
}
 
string asciiToBinary(const string &text) {
    string bin = "";
    for (char c : text) {
        for (int i = 7; i >= 0; i--)
            bin += ((c >> i) & 1) ? '1' : '0';
    }
    return bin;
}
 
int main() {
    string message, generator;
    cout << "Enter ASCII message: ";
    getline(cin, message);
    cout << "Enter generator polynomial (bits, e.g., 1101): ";
    cin >> generator;
 
    string data = asciiToBinary(message);
    int n = generator.size();
 
    
    string appended = data + string(n - 1, '0');
    string crc = mod2Divide(appended, generator);
    string frame = data + crc;
    cout << "\nData bits        : " << data;
    cout << "\nCRC (checksum)    : " << crc;
    cout << "\nTransmitted frame : " << frame << endl;
 
   
    char choice;
    cout << "\nSimulate a transmission error? (y/n): ";
    cin >> choice;
    string received = frame;
    if (choice == 'y' || choice == 'Y') {
        int pos;
        cout << "Enter bit position to flip (0-" << frame.size() - 1 << "): ";
        cin >> pos;
        received[pos] = (received[pos] == '0') ? '1' : '0';
        cout << "Received (with error): " << received << endl;
    }
 
   
    string remainder = mod2Divide(received, generator);
    bool hasError = remainder.find('1') != string::npos;
 
    cout << "\nReceiver-side remainder: " << remainder << endl;
    if (hasError)
        cout << "Result: Error detected in received data. Requesting retransmission." << endl;
    else
        cout << "Result: No error detected. Data accepted." << endl;
 
    return 0;
}
