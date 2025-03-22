#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

map<pair<string, string>, string> transitions;
set<string> acceptStates;
vector<string> states;
set<string> alphabet;
string startState;

void loadDFA(const string &filename)
{
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        if (line.find("States:") == 0)
        {
            istringstream iss(line.substr(7));
            string state;
            while (getline(iss, state, ','))
            {
                state.erase(0, state.find_first_not_of(" ")); // Trim leading space
                states.push_back(state);
            }
        }
        else if (line.find("Alphabet:") == 0)
        {
            istringstream iss(line.substr(9));
            string symbol;
            while (getline(iss, symbol, ','))
            {
                symbol.erase(0, symbol.find_first_not_of(" "));
                alphabet.insert(symbol);
            }
        }
        else if (line.find("Start:") == 0)
        {
            startState = line.substr(6);
            startState.erase(0, startState.find_first_not_of(" "));
        }
        else if (line.find("Accept:") == 0)
        {
            istringstream iss(line.substr(7));
            string acc;
            while (getline(iss, acc, ','))
            {
                acc.erase(0, acc.find_first_not_of(" "));
                acceptStates.insert(acc);
            }
        }
        else if (line.find("Transitions:") == 0)
        {
            continue;
        }
        else if (!line.empty())
        {
            istringstream iss(line);
            string fromState, inputSymbol, toState;
            iss >> fromState >> inputSymbol >> toState;
            transitions[{fromState, inputSymbol}] = toState;
        }
    }
    file.close();
}

bool isValidMoneyInput(const string &input)
{
    return (input == "1000" || input == "2000" || input == "5000" || input == "10000");
}

int main()
{
    loadDFA("vending_dfa.txt");

    string currentState = startState;
    vector<string> path;
    path.push_back(currentState);

    string input;
    int total = 0;

    while (true)
    {
        cout << "Masukkan uang atau beli minuman (1000, 2000, 5000, 10000, A, B, C): ";
        cin >> input;

        // Cek apakah input adalah tombol minuman
        if (input == "A" || input == "B" || input == "C")
        {
            int harga = 0;
            if (input == "A")
                harga = 3000;
            else if (input == "B")
                harga = 4000;
            else if (input == "C")
                harga = 6000;

            cout << "Lintasan DFA: ";
            for (size_t i = 0; i < path.size(); ++i)
            {
                cout << path[i];
                if (i != path.size() - 1)
                    cout << " -> ";
            }
            cout << endl;

            if (total == harga)
            {
                cout << "Minuman " << input << " dapat dibeli. Status: ACCEPTED." << endl;
            }
            else if (total < harga)
            {
                cout << "Uang tidak cukup. Status: REJECTED." << endl;
            }
            else
            {
                // Bonus case kembalian
                int kembalian = total - harga;
                cout << "Minuman " << input << " dapat dibeli. Status: ACCEPTED." << endl;
                cout << "Kembalian: " << kembalian << endl;
            }
            break; // Selesai transaksi
        }

        // Validasi input uang
        if (!isValidMoneyInput(input))
        {
            cout << "Input tidak valid! Hanya menerima: 1000, 2000, 5000, 10000, A, B, C." << endl;
            continue;
        }

        int uangMasuk = stoi(input);
        if (total + uangMasuk > 10000)
        {
            cout << "Total uang melebihi Rp10000. Transaksi tidak diterima." << endl;
            break;
        }

        // Transisi DFA
        if (transitions.find({currentState, input}) != transitions.end())
        {
            currentState = transitions[{currentState, input}];
            path.push_back(currentState);
            total += uangMasuk;
            cout << "Uang total: Rp" << total << endl;

            if (uangMasuk >= 0)
            { // Hanya cek ON kalau habis masukin uang
                if (total >= 6000)
                    cout << "ON: Minuman A, Minuman B, Minuman C" << endl;
                else if (total >= 4000)
                    cout << "ON: Minuman A, Minuman B" << endl;
                else if (total >= 3000)
                    cout << "ON: Minuman A" << endl;
            }
        }
        else
        {
            cout << "Transisi tidak valid. Tidak bisa memproses lebih dari Rp10000." << endl;
            break;
        }
    }

    return 0;
}
