#include<iostream>

#include<fstream>

#include<string>

#include<vector>

#include<iomanip>

using namespace std;

enum enMainMenueOptions { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6 };

const string ClientsFileName = "Clients.txt";

void ShowMainScreen();

void ClientNotFoundMessage(string AccountNumber) {
    cout << "Client With Account Number [" << AccountNumber << "] Is Not Found  !" << endl;
}

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector < string > SplitString(string S1, string Delim) {
    vector < string > vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "") {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") {

    sClient Client;
    vector < string > vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector < sClient > LoadCleintsDataFromFile(string FileName) {
    vector < sClient > vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string Line;
        sClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector < sClient > vClients, sClient& Client) {
    for (sClient C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector < sClient >& vClients) {
    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector < sClient > SaveCleintsDataToFile(string FileName, vector < sClient > vClients) {
   
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecord(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void backToHome()
{
    cout << "Press Any Key To Go Back To Home Screen  !" << endl;
    char chr;
    cin >> chr;
    system("cls");

    ShowMainScreen();
}

void ShowClientsList(string ClientsFileName) {
    system("cls");
    vector <sClient > vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (sClient Client : vClients) {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    backToHome();
}

bool CheckAccountNumberExists(string NewAccountNumber)
{
    vector <sClient> Vclients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient& c : Vclients) {
        if (c.AccountNumber == NewAccountNumber) {

            return true;
        }
    }
    return false;
}

sClient ReadNewClient() {
    sClient Client;
    do {
        cout << "Enter Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    } while (CheckAccountNumberExists(Client.AccountNumber));

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddDataLineToFile(string FileName, string stDataLine) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open()) {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void AddClients() {

    system("cls");
    string lineData;

    cout << "-----------------------" << endl;
    cout << "\t \t Add New Clients To Screen  \t \t " << endl;
    cout << "-----------------------" << endl;
    cout << "Adding New Client :" << endl;
    lineData = ConvertRecordToLine(ReadNewClient(), "#//#");
    AddDataLineToFile(ClientsFileName, lineData);
    backToHome();
}

void PrintClientCard(sClient Client) { 
    cout << "\nThe following are the client details:\n"; 
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode; 
    cout << "\nName         : " << Client.Name;  
    cout << "\nPhone        : " << Client.Phone;  
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector < sClient >& vClients) {
    sClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? " <<endl;
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {

            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully." << endl ;
            return true;

        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!" <<endl;
        return false;
    }
}

void DeleteClient()
{
    system("cls");
    string accountnumber;
    cout << "-----------------" << endl;
    cout << " \t \t Delete Client Screen \t \t " << endl;
    cout << "-----------------" << endl;
    cout << "Please Enter Account Number " << endl;
    vector <sClient> Vclients = LoadCleintsDataFromFile(ClientsFileName);
    cin >> accountnumber;
    if (CheckAccountNumberExists(accountnumber)) {

        DeleteClientByAccountNumber(accountnumber, Vclients);
        backToHome();
    }
    else {
        ClientNotFoundMessage(accountnumber);
        backToHome();
    }

}

string ReadClientAccountNumber() { 
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

sClient ReadNewClientWithoutAccount(string accountnumber) {
    sClient Client;

    Client.AccountNumber = accountnumber;
    cout << "Enter PinCode? " << endl;
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? " << endl;
    getline(cin, Client.Name);
    cout << "Enter Phone? " << endl;
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? " << endl;
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateClient() {
    system("cls");
    cout << "-----------------" << endl;
    cout << " \t \t Update Client Info Screen \t \t " << endl;
    cout << "-----------------" << endl;
    cout << "Please Enter Account Number " << endl;
    char option;
    vector <sClient> VClients =  LoadCleintsDataFromFile(ClientsFileName);
    string enteredAccountnumber= ReadClientAccountNumber();
    sClient Client;

    if (FindClientByAccountNumber(enteredAccountnumber,VClients, Client)) {
       
        PrintClientCard(Client);
        cout << "\n Are You Sure you Want To Update This Client ?! " << endl;
        cin >> option;
        if (option == 'y' || option == 'Y')
        {
            for (sClient& C : VClients) 
            {
                if (C.AccountNumber == enteredAccountnumber)
                {
                    C = ReadNewClientWithoutAccount(enteredAccountnumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, VClients);
            backToHome();
        }
        else {
            backToHome();
        }
    }
    else {
        ClientNotFoundMessage(enteredAccountnumber);
        backToHome();
    }
}

void FindClient() {
    system("cls");
    string accountnumber;
    sClient Client;
    vector <sClient> vClients=LoadCleintsDataFromFile(ClientsFileName);
    cout << "-----------------" << endl;
    cout << " \t \t Find Client Screen \t \t " << endl;
    cout << "-----------------" << endl;
    cout << "Please Enter Account Number " << endl;
    cin >> accountnumber;
    if (FindClientByAccountNumber(accountnumber, vClients, Client)) {
        PrintClientCard(Client) ;
    }
    else {
        ClientNotFoundMessage(accountnumber);
    }
    cout << endl;
    backToHome();
}

void ExitProgram()
{
    system("cls");
    exit(0);
}

void FunctionsRouter(short option)
{
    switch (option)
    {
    case 1:
        return ShowClientsList(ClientsFileName);
    case 2:
        return AddClients();
    case 3:
        return DeleteClient();
    case 4:
        return UpdateClient();
    case 5:
        return FindClient();
    case 6:
        return ExitProgram();

    }
}

void ShowMainScreen() {
    system("cls");
    short option;
    cout << "===========================================================================================" << endl;
    cout << setw(15) << "\t\t\t\t\t   Main Menue Screen  \t \t \t \t  " << endl;
    cout << "============================================================================================" << endl;
    cout << "\t [1] Show Client List " << endl;
    cout << "\t [2] Add New Client " << endl;
    cout << "\t [3] Delete Client " << endl;
    cout << "\t [4] Update Client Info " << endl;
    cout << "\t [5] Find Client " << endl;
    cout << "\t [6] Exit " << endl;
    cout << "=========================================" << endl;
    cout << "Choose What do you Want to Do ?  [ 1 To 6 ]" << endl;
    cin >> option;
    FunctionsRouter(option);

}


int main() {

    ShowMainScreen();
    system("pause>0");

    return 0;
}