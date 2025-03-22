#include <iostream> //membaca input & output
#include <fstream> //membaca file state dfa
#include <map> //menyimpan transisi dfa
#include <set> //menyimpan state yang diterima dan alphabet yang valid
#include <vector> //menyimpan daftar state dan jalur dfa
#include <sstream> //memproses teks yang dibaca dari file

using namespace std;

//deklarasi struktur data global//
map<pair<string, string>, string> transitions; //peta yang menyimpan transisi dfa
set<string> acceptStates; //menyimpan kumpulan state yang diterima
vector<string> states; //menyimpan semua daftar state yang diterima
set<string> alphabet; //menyimpan kumpulan alphabet yang diperbolehkan
string startState; //state awal dfa

void loadDFA(const string &filename) { //fungsi untuk membaca file konfigurasi [vending_dfa.txt]
    ifstream file(filename); //membuka file untuk dibaca
    string line; 

    while (getline(file, line)) { //membaca satu baris setiap perulangan
        if (line.find("States:") == 0) { //jika menemukan states → if diproses
            istringstream iss(line.substr(7)); //menghilangkan substring "States:"
            string state; //deklarasi variabel
            
            while (getline(iss, state, ',')) { //membaca state yang dipisah koma
                state.erase(0, state.find_first_not_of(" ")); //menghapus spasi di awal
                states.push_back(state); //menyimpan state ke dalam 'states'
            }
        }
        else if (line.find("Alphabet:") == 0) { //jika menemukan alphabet → else if diproses
            istringstream iss(line.substr(9)); //menghilangkan substring "Alphabet:"
            string symbol; //deklarasi variabel
            
            while (getline(iss, symbol, ',')) { //membaca simbol yang dipisah koma
                symbol.erase(0, symbol.find_first_not_of(" ")); //menghapus spasi di awal
                alphabet.insert(symbol); //menyimpan simbol ke dalam 'alphabet'
            }
        }
        else if (line.find("Start:") == 0) { //jika menemukan start → else if diproses
            startState = line.substr(6); //menghilangkan substring "Start:"
            startState.erase(0, startState.find_first_not_of(" ")); //menghapus spasi di awal
        }
        else if (line.find("Accept:") == 0) { //jika menemukan accept → else if diproses
            istringstream iss(line.substr(7)); //menghilangkan substring "Accept:"
            string acc; //deklarasi variabel
            
            while (getline(iss, acc, ',')) { //membaca state yang dapat diterima yang dipisah koma
                acc.erase(0, acc.find_first_not_of(" ")); //menghapus spasi di awal
                acceptStates.insert(acc); //menyimpan state ke dalam 'acceptStates'
            }
        }
        else if (line.find("Transitions:") == 0) { //jika menemukan transitions → else if diproses
            continue; //string 'Transitions' diabaikan [karena header]
        } 
        else if (!line.empty()) { //jika baris bukan header dan tidak kosong maka diproses sebagai transisi
            istringstream iss(line); //membaca kata per kata dari sebuah string line
            string fromState, inputSymbol, toState; //deklarasi variabel
            
            //membaca tiga data dari string line dan disimpan dalam tiga variabel berbeda
            //fromState: menyimpan state asal [dari mana dfa berpindah]
            //inputSymbol: input yang menyebabkan perubahan
            //toState: state tujuan [ke mana dfa berpindah setelah menerima input]
            iss >> fromState >> inputSymbol >> toState; 

            //map yang menyimpan aturan perpindahan dfa [transisi]
            transitions[{fromState, inputSymbol}] = toState;
        }
    }
    file.close(); //file ditutup
}

bool isValidMoneyInput(const string &input) { //memastikan uang yang dimasukkan sesuai aturan
    return (input == "1000" || input == "2000" || input == "5000" || input == "10000");
}

int main() {
    loadDFA("vending_dfa.txt"); //memanggil fungsi loadDFA() dan diminta untuk membaca file vending_dfa.txt
    string currentState = startState; //menginisialisasi state awal dfa 
    vector<string> path; //vektor untuk menyimpan lintasan dfa selama transaksi
    path.push_back(currentState);//menyimpan state awal

    string input; //menyimpan input berupa uang atau jenis minuman
    int total = 0;

    while (true) { //loop utama yang memproses input dari pengguna
        cout << "Masukkan uang atau beli minuman (1000, 2000, 5000, 10000, A, B, C): ";
        cin >> input; 

        //cek apakah input adalah jenis minuman
        if (input == "A" || input == "B" || input == "C") {
            int harga = 0;
           
            if (input == "A")
                harga = 3000;
            else if (input == "B")
                harga = 4000;
            else if (input == "C")
                harga = 6000;

            cout << "Lintasan DFA: ";
            for (size_t i = 0; i < path.size(); ++i) { //mencetak lintasan dfa
                cout << path[i];
                if (i != path.size() - 1)
                    cout << " -> ";
            }
            cout << endl;

            if (total == harga) {
                cout << "Minuman " << input << " dapat dibeli. Status: ACCEPTED." << endl;
            } else if (total < harga) {
                cout << "Uang tidak cukup. Status: REJECTED." << endl;
            } else { // kasus asli: kondisi dimana uang != harga minuman [lebih]
                cout << "Uang lebih dari harga minuman. Status: REJECTED." << endl;
            }  break; //transaksi berakhir
        }

        if (!isValidMoneyInput(input)) {  //validasi input uang 
            cout << "Input tidak valid! Hanya menerima: 1000, 2000, 5000, 10000, A, B, C." << endl;
            continue;
        }

        int uangMasuk = stoi(input); //mengubah input uang yang berupa string menjadi int 
        if (total + uangMasuk > 10000) {
            cout << "Total uang melebihi Rp10000. Transaksi tidak diterima." << endl;
            break;
        }

        //proses transisi dfa
        if (transitions.find({currentState, input}) != transitions.end()) { 
        //cek kondisi apakah ada transisi yang sesuai dengan aturan dfa berdasarkan state saat ini dan input
            currentState = transitions[{currentState, input}]; //melakukan update pada 'state saat ini'
            path.push_back(currentState); //menyimpan state baru ke dalam path
            total += uangMasuk; //jumlah total uang yang telah diinput pengguna
            cout << "Uang total: Rp" << total << endl;

            if (uangMasuk >= 0) { //menampilkan jenis minuman yang dapat dibeli berdasarkan total uang pengguna
                if (total >= 6000)
                    cout << "ON: Minuman A, Minuman B, Minuman C" << endl;
                else if (total >= 4000)
                    cout << "ON: Minuman A, Minuman B" << endl;
                else if (total >= 3000)
                    cout << "ON: Minuman A" << endl;
            }
        }
        else { //bekerja ketika kondisi input tidak valid
            cout << "Transisi tidak valid. Tidak bisa memproses lebih dari Rp10000." << endl;
            break;
        }
    }
    return 0;
}
