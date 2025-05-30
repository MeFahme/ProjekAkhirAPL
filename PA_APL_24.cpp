#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm> 
#include <cctype>
#include <stdexcept>
using namespace std;

const int MAX_SIRKUIT = 100;
const int MAX_USERS = 100;

struct Pengelola {
    string namaPengelola;
    string kontak;
};

struct Sirkuit {
    string nama;
    string lokasi;
    string statusReservasi;
    Pengelola pengelola;
    int harga;
    string reservasiOleh;
    string tanggalReservasi;
};

Sirkuit sirkuit[MAX_SIRKUIT] = {
    {"Mandalika", "Pulau Lombok", "Tersedia", {"PT Mandalika Racing", "087421974638"}, 5000000, "", ""},
    {"Internasional Sentul", "Bogor", "Tersedia", {"Sentul Management", "081239587264"}, 3000000, "", ""},
    {"Jaya Ancol", "Jakarta", "Tersedia", {"Ancol Circuit Ltd", "089973214186"}, 3500000, "", ""},
    {"e-Prix", "Jakarta", "Tersedia", {"e-Prix Inter", "083150642987"}, 4000000, "", ""},
    {"Bukit Peusar", "Tasikmalaya", "Tersedia", {"Peusar Motorsports", "085276431059"}, 2500000, "", ""},
    {"Gokart Subang", "Subang", "Tersedia", {"Subang Racing Club", "089821507386"}, 2000000, "", ""},
    {"Pancing Circuit", "Medan", "Tersedia", {"Medan Racing Team", "082163945728"}, 2800000, "", ""},
    {"Sirkuit Skyland", "Palembang", "Tersedia", {"Skyland Motorsport", "086048273195"}, 2700000, "", ""},
    {"Sirkuit Balipat", "Banjarmasin", "Tersedia", {"Balipat Racing Club", "081597306241"}, 2600000, "", ""}
};

int panjang = 9;

struct User {
    string username;
    string password;
};

User users[MAX_USERS];
int jumlahPengguna = 0;
string currentUser;
string adminUsername = "admin";
string adminPassword = "admin1";

// Fungsi bantu
int inputAngka(const string& pesan) {
    int nilai;
    string input;
    while (true) {
        cin.clear();

        cout << pesan;
        getline(cin, input);

        try {
            size_t pos;
            nilai = stoi(input, &pos);
            if (pos == input.size()) break;
        } catch (...) {
            cout << "input harus angka!\n";
        }
    }
    return nilai;
}

void muatAkunDariFile() {
    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(":");
        if (pos != string::npos && jumlahPengguna < MAX_USERS) {
            users[jumlahPengguna].username = line.substr(0, pos);
            users[jumlahPengguna].password = line.substr(pos + 1);
            jumlahPengguna++;
        }
    }
    file.close();
}

void simpanAkunKeFile() {
    ofstream file("users.txt");
    for (int i = 0; i < jumlahPengguna; i++) {
        file << users[i].username << ":" << users[i].password << endl;
    }
    file.close();
}

// Histori Reservasi
struct HistoriReservasi {
    string namaSirkuit;
    string username;
    string status;
    string tanggal;
};

const int MAX_HISTORI = 100;
HistoriReservasi histori[MAX_HISTORI];
int jumlahHistori = 0;

void muatHistoriDariFile() {
    ifstream file("histori_reservasi.txt");
    string line;
    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 != string::npos && p2 != string::npos && p3 != string::npos && jumlahHistori < MAX_HISTORI) {
            histori[jumlahHistori].namaSirkuit = line.substr(0, p1);
            histori[jumlahHistori].username = line.substr(p1 + 1, p2 - p1 - 1);
            histori[jumlahHistori].status = line.substr(p2 + 1, p3 - p2 - 1);
            histori[jumlahHistori].tanggal = line.substr(p3 + 1);
            jumlahHistori++;
        }
    }
    file.close();
}

void simpanKeHistori(string nama, string user, string status, string tanggal) {
    if (jumlahHistori < MAX_HISTORI) {
        histori[jumlahHistori].namaSirkuit = nama;
        histori[jumlahHistori].username = user;
        histori[jumlahHistori].status = status;
        histori[jumlahHistori].tanggal = tanggal;
        jumlahHistori++;
    }
    ofstream file("histori_reservasi.txt", ios::app);
    file << nama << "|" << user << "|" << status << "|" << tanggal << "\n";
    file.close();
}

// Histori Reservasi
void lihatHistoriReservasi() {
    system("cls");
    cout << "+------------------------+------------------+---------------------+------------+\n";
    cout << "|     Nama Sirkuit       |     Pengguna     |      Status         |  Tanggal   |\n";
    cout << "+------------------------+------------------+---------------------+------------+\n";
    for (int i = 0; i < jumlahHistori; i++) {
        cout << "| " << setw(22) << left << histori[i].namaSirkuit << " | "
             << setw(16) << left << histori[i].username << " | "
             << setw(19) << left << histori[i].status << " | "
             << setw(10) << left << histori[i].tanggal << " |\n";
    }
    cout << "+------------------------+------------------+---------------------+------------+\n";
    system("pause");
}

//cari histori reservasi
void cariHistoriByTanggal() {
    lihatHistoriReservasi();
    string tanggalCari;
    
    // Validasi input tanggal tidak boleh kosong
    do {
        cout << "Masukkan tanggal (dd atau dd/mm atau dd/mm/yyyy): ";
        getline(cin, tanggalCari);

        if (tanggalCari.empty()) {
            cout << "Input tidak boleh kosong!\n";
        }
    } while (tanggalCari.empty());

    bool ketemu = false;

    // Validasi panjang input
    if (tanggalCari.length() != 2 && tanggalCari.length() != 5 && tanggalCari.length() != 10) {
        cout << "Format tanggal tidak sesuai! Harap masukkan:\n";
        cout << "- Hari saja (contoh: 02)\n";
        cout << "- Hari/Bulan (contoh: 02/12)\n";
        cout << "- Tanggal lengkap (contoh: 02/12/2024)\n";
        system("pause");
        return;
    }

    
    cout << "\n+------------------------+------------------+---------------------+------------+\n";
    cout << "|      Nama Sirkuit      |     Pengguna     |        Status       |   Tanggal  |\n";
    cout << "+------------------------+------------------+---------------------+------------+\n";

for (int i = 0; i < jumlahHistori; i++) {
        string tgl = histori[i].tanggal;
        if (tanggalCari.length() == 2) {
            if (tgl.substr(0, 2) == tanggalCari) {
                ketemu = true;
                cout << "| " << setw(22) << left << histori[i].namaSirkuit << " | "
                     << setw(16) << left << histori[i].username << " | "
                     << setw(19) << left << histori[i].status << " | "
                     << setw(10) << left << histori[i].tanggal << " |\n";
            }
        } else if (tanggalCari.length() == 5) {
            if (tgl.substr(0, 5) == tanggalCari) {
                ketemu = true;
                cout << "| " << setw(22) << left << histori[i].namaSirkuit << " | "
                     << setw(16) << left << histori[i].username << " | "
                     << setw(19) << left << histori[i].status << " | "
                     << setw(10) << left << histori[i].tanggal << " |\n";
            }
        } else if (tanggalCari.length() == 10) {
            if (tgl == tanggalCari) {
                ketemu = true;
                cout << "| " << setw(22) << left << histori[i].namaSirkuit << " | "
                     << setw(16) << left << histori[i].username << " | "
                     << setw(19) << left << histori[i].status << " | "
                     << setw(10) << left << histori[i].tanggal << " |\n";
            }
        }
    }

    cout << "+------------------------+------------------+---------------------+------------+\n";

    if (!ketemu) {
        cout << "Tidak ditemukan histori pada tanggal tersebut.\n";
    }
    system("pause");
}

// Menampilkan list sirkuit
void tampilkanSirkuitRekursif(Sirkuit sirkuit[], int index, int jumlahSirkuit) {
    if (index >= jumlahSirkuit) return;
    cout << "| " << setw(3) << right << index + 1 << "   | "
         << setw(22) << left << sirkuit[index].nama << " | "
         << setw(14) << left << sirkuit[index].lokasi << " | "
         << setw(22) << left << sirkuit[index].pengelola.namaPengelola << " | "
         << setw(25) << left << sirkuit[index].pengelola.kontak << " | "
         << setw(16) << left << sirkuit[index].statusReservasi << " | "
         << setw(4) << right << "Rp. " << sirkuit[index].harga << "   |\n";
    tampilkanSirkuitRekursif(sirkuit, index + 1, jumlahSirkuit);
}

void tampilkanSirkuit() {
    system("cls");
    cout << "\n+-------+------------------------+----------------+------------------------+---------------------------+------------------+---------------+\n";
    cout << "|  No   |      Nama Sirkuit      |     Lokasi     |      Pengelola         |          Kontak           | Status Reservasi |     Harga     |\n";
    cout << "+-------+------------------------+----------------+------------------------+---------------------------+------------------+---------------+\n";
    tampilkanSirkuitRekursif(sirkuit, 0, panjang);
    cout << "+-------+------------------------+----------------+------------------------+---------------------------+------------------+---------------+\n";
    system("pause");
}


bool checkUsername(string username) {
    for (int i = 0; i < jumlahPengguna; i++) {
        if (users[i].username == username) {
            return true;
        }
    }
    return false;
}

// Registrasi
void registerUser() {
    if (jumlahPengguna >= MAX_USERS) {
        cout << "Batas pengguna tercapai!\n";
    } else {
        string username, password;

        system("cls");
        cout << "=============================\n";
        cout << "           REGISTER          \n";
        cout << "=============================\n";

        try {
            do {
                cout << "Masukkan Username: ";
                getline(cin, username);
                if (username.empty()) throw runtime_error("Username tidak boleh kosong!");
                if (checkUsername(username)) throw runtime_error("Username telah digunakan!");
            } while (username.empty());

            do {
                cout << "Masukkan Password: ";
                getline(cin, password);
                if (password.empty()) throw runtime_error("Password tidak boleh kosong!");
            } while (password.empty());

            users[jumlahPengguna].username = username;
            users[jumlahPengguna].password = password;
            jumlahPengguna++;
            simpanAkunKeFile();
            cout << "Registrasi Berhasil!\n";

        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
    system("pause");
}

bool loginUser(bool &isLogin) {
    int login = 3;
    string username, password;
    while (login > 0) {
        system("cls");
        cout << "=============================\n";
        cout << "            LOGIN            \n";
        cout << "=============================\n";

        try {
            cout << "Masukkan Username: ";
            getline(cin, username);
            if (username.empty()) throw runtime_error("Username tidak boleh kosong!");

            cout << "Masukkan Password: ";
            getline(cin, password);
            if (password.empty()) throw runtime_error("Password tidak boleh kosong!");

            if (username == adminUsername && password == adminPassword) {
                isLogin = true;
                currentUser = username;
                return true;
            }

            for (int i = 0; i < jumlahPengguna; i++) {
                if (users[i].username == username && users[i].password == password) {
                    currentUser = username;
                    return true;
                }
            }

            login--;
            cout << "\nGagal! Percobaan tersisa: " << login << "\n";
            system("pause");

        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
            system("pause");
        }
    }
    return false;
}

// Fungsi bantu: Validasi format tanggal dd/mm/yyyy
bool isValidTanggal(const string &tanggal) {
    try {
        if (tanggal.size() != 10) throw runtime_error("Format harus dd/mm/yyyy");

        if (tanggal[2] != '/' || tanggal[5] != '/') throw runtime_error("Format salah: dd/mm/yyyy");

        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;
            if (!isdigit(tanggal[i])) throw runtime_error("Tanggal hanya boleh berisi angka");
        }

        int hari = stoi(tanggal.substr(0, 2));
        int bulan = stoi(tanggal.substr(3, 2));
        int tahun = stoi(tanggal.substr(6, 4));

        if (tahun < 2024 || tahun > 2100) throw runtime_error("Tahun harus antara 2024-2100");
        if (bulan < 1 || bulan > 12) throw runtime_error("Bulan harus antara 1-12");
        if (hari < 1 || hari > 31) throw runtime_error("Hari harus antara 1-31");

        return true;
    } catch (...) {
        return false;
    }
}

// Reservasi Sirkuit
void reservasiSirkuit() {
    system("cls");
    tampilkanSirkuit();
    cout << "Masukkan nomor sirkuit yang ingin direservasi: ";
    int index = inputAngka("");
    if (index > 0 && index <= panjang) {
        if (sirkuit[index - 1].statusReservasi == "Tersedia") {
            string tanggal;
            bool valid = false;

            do {
                cout << "Masukkan tanggal reservasi (format: dd/mm/yyyy): ";
                cin >> tanggal;

                if (isValidTanggal(tanggal)) {
                    valid = true;
                } else {
                    cout << "Format tanggal salah! Harap masukkan dalam format dd/mm/yyyy.\n";
                }
            } while (!valid);

            sirkuit[index - 1].statusReservasi = "Menunggu";
            sirkuit[index - 1].reservasiOleh = currentUser;
            sirkuit[index - 1].tanggalReservasi = tanggal;

            simpanKeHistori(sirkuit[index - 1].nama, currentUser, "Menunggu", tanggal);
            cout << "Reservasi berhasil diajukan. Harap menunggu konfirmasi admin.\n";
        } else {
            cout << "Maaf, sirkuit sedang tidak tersedia.\n";
        }
    } else {
        cout << "Nomor tidak valid.\n";
    }
    system("pause");
}

// Konfirmasi Reservasi (Admin)
void konfirmasiReservasi() {
    system("cls");
    cout << "+-------+------------------------+---------------------+---------------+------------------+-------------------+\n";
    cout << "| No    |      Nama Sirkuit      |   Status Reservasi  |     Harga     | Direservasi Oleh | Tanggal Reservasi |\n";
    cout << "+-------+------------------------+---------------------+---------------+------------------+-------------------+\n";
    bool hasPending = false;
    for (int i = 0; i < panjang; i++) {
        if (sirkuit[i].statusReservasi == "Menunggu") {
            hasPending = true;
            cout << "| " << setw(3) << right << i + 1 << "   | "
                 << setw(22) << left << sirkuit[i].nama << " | "
                 << setw(16) << left << sirkuit[i].statusReservasi << "    | "
                 << setw(4) << right << "Rp." << sirkuit[i].harga << "   | "
                 << setw(15) << left << sirkuit[i].reservasiOleh << "  | "
                 << setw(15) << left << sirkuit[i].tanggalReservasi << "   |\n";
        }
    }
    cout << "+-------+------------------------+---------------------+---------------+------------------+-------------------+\n";
    if (!hasPending) {
        cout << "Tidak ada reservasi.\n";
        system("pause");
        return;
    }
    int index = inputAngka("Pilih nomor sirkuit untuk konfirmasi: ");
    if (index > 0 && index <= panjang && sirkuit[index - 1].statusReservasi == "Menunggu") {
        int confirmInput = inputAngka("[1] Konfirmasi\n[2] Tolak\nPilih tindakan (1/2): ");
        string statusAkhir;
        if (confirmInput == 1) {
            sirkuit[index - 1].statusReservasi = "Digunakan";
            statusAkhir = "Dikonfirmasi";
        } else if (confirmInput == 2) {
            sirkuit[index - 1].statusReservasi = "Tersedia";
            statusAkhir = "Ditolak";
        } else {
            cout << "Input tidak valid!\n";
            system("pause");
            return;
        }

        // Update histori
        bool ditemukan = false;
        for (int i = 0; i < jumlahHistori; i++) {
            if (histori[i].namaSirkuit == sirkuit[index - 1].nama &&
                histori[i].username == sirkuit[index - 1].reservasiOleh &&
                histori[i].tanggal == sirkuit[index - 1].tanggalReservasi) {
                histori[i].status = statusAkhir;
                ditemukan = true;
                break;
            }
        }
        if (!ditemukan) {
            simpanKeHistori(sirkuit[index - 1].nama, sirkuit[index - 1].reservasiOleh, statusAkhir, sirkuit[index - 1].tanggalReservasi);
        }

        ofstream file("histori_reservasi.txt");
        for (int i = 0; i < jumlahHistori; i++) {
            file << histori[i].namaSirkuit << "|" 
                 << histori[i].username << "|" 
                 << histori[i].status << "|" 
                 << histori[i].tanggal << "\n";
        }
        file.close();   
        cout << "Status berhasil diperbarui dan disimpan ke file.\n";
    } else {
        cout << "Nomor tidak valid atau sirkuit tidak sedang menunggu konfirmasi.\n";
    }
    system("pause");
}


// Lihat Status Reservasi (User)
void lihatStatusReservasi() {
    system("cls");
    cout << "+-------+------------------------+---------------------+---------------+-------------------+\n";
    cout << "| No    | Nama Sirkuit           | Status Reservasi    | Harga         | Tanggal Reservasi |\n";
    cout << "+-------+------------------------+---------------------+---------------+-------------------+\n";
    bool found = false;
    for (int i = 0; i < panjang; i++) {
        if (sirkuit[i].reservasiOleh == currentUser) {
            cout << "|  " << setw(1) << i+1 << "    | "
                 << setw(22) << left << sirkuit[i].nama << " | "
                 << setw(16) << left << sirkuit[i].statusReservasi << "    | "
                 << setw(4) << right << "Rp. " << sirkuit[i].harga << "   | "
                 << setw(15) << left << sirkuit[i].tanggalReservasi << "   |\n";
            found = true;
        }
    }
    cout << "+-------+------------------------+---------------------+---------------+-------------------+\n";
    if (!found) cout << "Anda belum memiliki reservasi.\n";
    system("pause");
}

// Menu Admin
void ubahStatusReservasi() {
    system("cls");
    cout << "\n+-----+------------------------+------------------+\n";
    cout << "| No  |      Nama Sirkuit      | Status Reservasi |\n";
    cout << "+-----+------------------------+------------------+\n";
    for (int i = 0; i < panjang; i++) {
        cout << "|  " << setw(1) << i+1 << "  | "
             << setw(22) << left << sirkuit[i].nama << " | "
             << setw(16) << left << sirkuit[i].statusReservasi << " |\n";
    }
    cout << "+-----+------------------------+------------------+\n";
    int index = inputAngka("Pilih nomor sirkuit: ");
    if (index > 0 && index <= panjang) {
        int statusInput = inputAngka("[1] Tersedia\n[2] Digunakan\nPilih Status Baru (1/2): ");
        if (statusInput == 1) sirkuit[index - 1].statusReservasi = "Tersedia";
        else if (statusInput == 2) sirkuit[index - 1].statusReservasi = "Digunakan";
        else cout << "Input tidak valid!\n";
        cout << "Status berhasil diubah menjadi: " << sirkuit[index - 1].statusReservasi << endl;
    } else {
        cout << "Nomor tidak valid!\n";
    }
    system("pause");
}

void tambahSirkuit(string nama, string lokasi, string namaPengelola, string kontak, int harga) {
    if (panjang >= MAX_SIRKUIT) {
        cout << "Batas jumlah sirkuit tercapai!" << endl;
    } else {
        // Validasi semua input tidak boleh kosong
        if (nama.empty() || lokasi.empty() || namaPengelola.empty() || kontak.empty()) {
            cout << "Error: Semua field harus diisi! Penambahan sirkuit dibatalkan." << endl;
        } else {
            sirkuit[panjang].nama = nama;
            sirkuit[panjang].lokasi = lokasi;
            sirkuit[panjang].pengelola.namaPengelola = namaPengelola;
            sirkuit[panjang].pengelola.kontak = kontak;
            sirkuit[panjang].harga = harga;
            sirkuit[panjang].statusReservasi = "Tersedia";
            panjang++;
            cout << "Sirkuit berhasil ditambahkan!" << endl;
        }
    }
    system("pause");
}

void hapusSirkuit() {
    system("cls");
    cout << "\n+-----+------------------------+----------------+\n";
    cout << "| No  |      Nama Sirkuit      |     Lokasi     |\n";
    cout << "+-----+------------------------+----------------+\n";
    for (int i = 0; i < panjang; i++) {
        cout << "|  " << setw(1) << i+1 << "  | "
             << setw(22) << left << sirkuit[i].nama << " | "
             << setw(14) << left << sirkuit[i].lokasi << " |\n";
    }
    cout << "+-----+------------------------+----------------+\n";
    int index = inputAngka("Pilih nomor sirkuit yang ingin dihapus: ");
    if (index > 0 && index <= panjang) {
        for (int i = index - 1; i < panjang - 1; i++) {
            sirkuit[i] = sirkuit[i + 1];
        }
        panjang--;
        cout << "Sirkuit berhasil dihapus!\n";
    } else {
        cout << "Nomor tidak valid!\n";
    }
    system("pause");
}

void ubahDetailSirkuit(Sirkuit& s) {
    system("cls");
    cout << "====================================\n";
    cout << "            Detail Sirkuit          \n";
    cout << "====================================\n";
    cout << "Nama          : " << s.nama << endl;
    cout << "Lokasi        : " << s.lokasi << endl;
    cout << "Pengelola     : " << s.pengelola.namaPengelola << endl;
    cout << "Kontak        : " << s.pengelola.kontak << endl;
    cout << "Harga         : Rp. " << s.harga << endl;
    cout << "\n====================================\n";
    cout << "             Detail Baru           \n";
    cout << "====================================\n";

    string nama, lokasi, namaPengelola, kontak;
    int harga;

    // Input Nama Sirkuit (Tidak Boleh Kosong)
    while (true) {
        cout << "Nama          : ";
        getline(cin, nama);
        if (nama.empty()) {
            cout << "Nama tidak boleh kosong!\n";
        } else {
            break;
        }
    }

    // Input Lokasi (Tidak Boleh Kosong)
    while (true) {
        cout << "Lokasi        : ";
        getline(cin, lokasi);
        if (lokasi.empty()) {
            cout << "Lokasi tidak boleh kosong!\n";
        } else {
            break;
        }
    }

    // Input Nama Pengelola (Tidak Boleh Kosong)
    while (true) {
        cout << "Pengelola     : ";
        getline(cin, namaPengelola);
        if (namaPengelola.empty()) {
            cout << "Nama pengelola tidak boleh kosong!\n";
        } else {
            break;
        }
    }

    // Input Kontak (Harus Angka & Tidak Boleh Kosong)
    while (true) {
        cout << "Kontak        : ";
        getline(cin, kontak);
        if (kontak.empty()) {
            cout << "Kontak tidak boleh kosong!\n";
        } else if (!all_of(kontak.begin(), kontak.end(), ::isdigit)) {
            cout << "Kontak harus berupa angka!\n";
        } else {
            break;
        }
    }

    // Input Harga (Harus Lebih dari 0)
    harga = inputAngka("Harga         : Rp. ");
    while (harga <= 0) {
        cout << "Harga harus lebih besar dari nol.\n";
        harga = inputAngka("Masukkan ulang harga: ");
    }

    // Simpan data ke objek setelah semua validasi berhasil
    s.nama = nama;
    s.lokasi = lokasi;
    s.pengelola.namaPengelola = namaPengelola;
    s.pengelola.kontak = kontak;
    s.harga = harga;
    s.statusReservasi = "Tersedia";

    cout << "\nDetail sirkuit berhasil diperbarui.\n";
    system("pause");
}

void manajemenSirkuit() {
    bool loop = true;
    while (loop) {
        system("cls");
        cout << "=============================\n";
        cout << "       MANAJEMEN SIRKUIT     \n";
        cout << "=============================\n";
        cout << "1. Ubah Status Reservasi\n";
        cout << "2. Tambah Sirkuit\n";
        cout << "3. Hapus Sirkuit\n";
        cout << "4. Ubah Detail Sirkuit\n";
        cout << "5. Kembali ke Menu Utama\n";
        int pilihan = inputAngka("Pilih submenu: ");
        
        switch(pilihan) {
            case 1: ubahStatusReservasi(); break;
            case 2: {
                tampilkanSirkuit();
                string nama, lokasi, pengelola, kontak;
                int harga;

                cout << "Masukkan Nama Sirkuit: ";
                cin.ignore();
                getline(cin, nama);
                while (nama.empty()) {
                    cout << "Nama tidak boleh kosong. ";
                    getline(cin, nama);
                }
                cout << "Masukkan Lokasi: ";
                getline(cin, lokasi);
                while (lokasi.empty()) {
                    cout << "Lokasi tidak boleh kosong.";
                    getline(cin, lokasi);
                }
                cout << "Masukkan Nama Pengelola: ";
                getline(cin, pengelola);
                while (pengelola.empty()) {
                    cout << "Nama Pengelola tidak boleh kosong.";
                    getline(cin, pengelola);
                }
                // Validasi kontak hanya angka
                kontak = "";
                while (true) {
                    cout << "Masukkan Kontak Pengelola: ";
                    getline(cin, kontak);
                    if (kontak.empty()) {
                        cout << "Kontak tidak boleh kosong.\n";
                    } else if (all_of(kontak.begin(), kontak.end(), ::isdigit)) {
                        break;
                    } else {
                        cout << "Kontak harus berupa angka!\n";
                    }
                }

                harga = inputAngka("Masukkan Harga Sewa Sirkuit: ");
                while (harga <= 0) {
                    cout << "Harga harus lebih besar dari nol.\n";
                    harga = inputAngka("Masukkan ulang harga: ");
                }

                tambahSirkuit(nama, lokasi, pengelola, kontak, harga);
                break;
            }
            case 3: hapusSirkuit(); break;
            case 4: {
                    system("cls");
                    cout << "\n+-----+------------------------+----------------+\n";
                    cout << "| No  |      Nama Sirkuit      |     Lokasi     |\n";
                    cout << "+-----+------------------------+----------------+\n";
                    for (int i = 0; i < panjang; i++) {
                        cout << "|  " << setw(1) << i+1 << "  | "
                            << setw(22) << left << sirkuit[i].nama << " | "
                            << setw(14) << left << sirkuit[i].lokasi << " |\n";
                    }
                    cout << "+-----+------------------------+----------------+\n";
                int idx = inputAngka("Pilih nomor sirkuit : ");
                if (idx > 0 && idx <= panjang) {
                    ubahDetailSirkuit(sirkuit[idx - 1]);
                } else {
                    cout << "Nomor tidak valid!\n";
                    system("pause");
                }
                break;
            }
            case 5: loop = false; break;
            default: cout << "Pilihan tidak valid!\n"; system("pause");
        }
    }
}

//dereference
void tampilkanPengelola(const Pengelola* p) {
    cout << "Nama Pengelola   : " << p->namaPengelola << endl;
    cout << "Kontak Pengelola : " << p->kontak << endl;
}

//bubble huruf ascend
void bubbleSortNama(Sirkuit sirkuit[], int panjang) {
    for (int i = 0; i < panjang - 1; i++) {
        for (int j = 0; j < panjang - i - 1; j++) {
            if (sirkuit[j].nama > sirkuit[j + 1].nama) {
                Sirkuit temp = sirkuit[j];
                sirkuit[j] = sirkuit[j + 1];
                sirkuit[j + 1] = temp;
            }
        }
    }
    cout << "\nDiurutkan berdasarkan Nama Sirkuit.\n";
    system("pause");
}

//insert huruf ascend
void insertionSortLokasi(Sirkuit sirkuit[], int panjang) {
    for (int i = 1; i < panjang; i++) {
        Sirkuit key = sirkuit[i];
        int j = i - 1;
        while (j >= 0 && sirkuit[j].lokasi > key.lokasi) {
            sirkuit[j + 1] = sirkuit[j];
            j--;
        }
        sirkuit[j + 1] = key;
    }
    cout << "\nDiurutkan berdasarkan Lokasi Sirkuit.\n";
    system("pause");
}

//selection angka descend
void selectionSortHarga(Sirkuit sirkuit[], int panjang) {
    for (int i = 0; i < panjang - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < panjang; j++) {
            if (sirkuit[j].harga > sirkuit[maxIndex].harga) {
                maxIndex = j; // Bandingkan berdasarkan harga
            }
        }
        if (maxIndex != i) {
            // Tukar data jika ditemukan harga lebih besar
            Sirkuit temp = sirkuit[i];
            sirkuit[i] = sirkuit[maxIndex];
            sirkuit[maxIndex] = temp;
        }
    }
    cout << "\nDiurutkan berdasarkan Harga Sewa." << endl;
    system("pause");
}

void menuSorting() {
    int pilihSort;
    do {
        system("cls");
        cout << "=========================================\n";
        cout << "               MENU SORTING              \n";
        cout << "=========================================\n";
        cout << "1. Urutkan Nama Sirkuit (Ascending)\n";
        cout << "2. Urutkan Lokasi Sirkuit (Ascending)\n";
        cout << "3. Urutkan Harga (Descending)\n";
        cout << "4. Kembali\n";
        cout << "Pilih menu: ";
        cin >> pilihSort;
        cin.ignore();

        switch (pilihSort) {
            case 1:
                bubbleSortNama(sirkuit, panjang);
                break;
            case 2:
                insertionSortLokasi(sirkuit, panjang);
                break;
            case 3:
                selectionSortHarga(sirkuit, panjang);
                break;
            case 4:
                break;
            default:
                cout << "Pilihan tidak valid!\n";
                system("pause");
        }
    } while (pilihSort != 4);
}

// Pencarian lokasi sirkuit
void cariSirkuitByLokasi() {
    tampilkanSirkuit();
    string lokasiCari;
    cout << "Masukkan lokasi sirkuit: ";

    cin.clear();
    getline(cin, lokasiCari);

    bool ketemu = false;

    cout << "+----+--------------------------+----------------------+---------------+\n";
    cout << "| No |      Nama Sirkuit        |        Lokasi        |     Harga     |\n";
    cout << "+----+--------------------------+----------------------+---------------+\n";


    for (int i = 0; i < panjang; i++) {
        if (sirkuit[i].lokasi == lokasiCari) {
            cout << "| " << setw(2) << right << i + 1 << " | "
                 << setw(24) << left << sirkuit[i].nama << " | "
                 << setw(20) << left << sirkuit[i].lokasi << " | "
                 << setw(4) << right << "Rp." << sirkuit[i].harga << "   |\n";
            ketemu = true;
        }
    }
    cout << "+----+--------------------------+----------------------+---------------+\n";

    if (!ketemu) {
        cout << "Tidak ada sirkuit dengan lokasi \"" << lokasiCari << "\"\n";
    }

    system("pause");
}

void menuAdmin() {
    bool loggedIn = true;
    while (loggedIn) {
        system("cls");
        cout << "====================================\n";
        cout << "              MENU ADMIN            \n";
        cout << "====================================\n";
        cout << "1. List Sirkuit\n";
        cout << "2. Manajemen Sirkuit\n";
        cout << "3. Konfirmasi Reservasi\n";
        cout << "4. Lihat Histori Reservasi\n";
        cout << "5. Cari Histori Berdasarkan Tanggal\n";
        cout << "6. Sorting\n";
        cout << "7. Logout\n";
        int pilihan = inputAngka("Pilih menu: ");

        switch(pilihan) {
            case 1: tampilkanSirkuit(); break;
            case 2: manajemenSirkuit(); break;
            case 3: konfirmasiReservasi(); break;
            case 4: lihatHistoriReservasi(); break;
            case 5: cariHistoriByTanggal(); break;
            case 6: menuSorting(); break;
            case 7: loggedIn = false; break;
            default: cout << "Pilihan tidak valid!\n"; system("pause");
        }
    }
}



// Menu User
void menuUser() {
    bool loggedIn = true;
    while (loggedIn) {
        system("cls");
        cout << "=============================\n";
        cout << "          MENU USER          \n";
        cout << "=============================\n";
        cout << "1. List Sirkuit\n";
        cout << "2. Reservasi Sirkuit\n";
        cout << "3. Lihat Status Reservasi\n";
        cout << "4. Cari Lokasi Sirkuit\n";
        cout << "5. Tampilkan Pengelola\n";
        cout << "6. Sorting\n";
        cout << "7. Logout\n";
        int pilihan = inputAngka("Pilih menu: ");
        if (pilihan == 1) tampilkanSirkuit();
        else if (pilihan == 2) reservasiSirkuit();
        else if (pilihan == 3) lihatStatusReservasi();
        else if (pilihan == 4) cariSirkuitByLokasi();
        else if (pilihan == 5){
            system("cls");
            cout << "=======================================\n";
            cout << "             INFO PENGELOLA            \n";
            cout << "=======================================\n";
            for (int i = 0; i < panjang; i++) {
                cout << i + 1 << ". " << sirkuit[i].nama << endl;
            }

            int index = inputAngka("\nMasukkan nomor sirkuit: ");
            if (index > 0 && index <= panjang) {
                tampilkanPengelola(&sirkuit[index - 1].pengelola);
            } else {
                cout << "Nomor sirkuit tidak valid!" << endl;
                system("pause");
            }
            system("pause");
        }
        else if (pilihan == 6) menuSorting();
        else if (pilihan == 7) loggedIn = false;
    }
}

// Program utama
int main() {
    muatAkunDariFile();
    muatHistoriDariFile();
    bool running = true;
    while (running) {
        system("cls");
        cout << "=============================\n";
        cout << "           WELCOME           \n";
        cout << "=============================\n";
        cout << "1. Register\n2. Login\n3. Keluar\nPilih: ";
        int pilihan = inputAngka("");
        switch (pilihan) {
            case 1: registerUser(); break;
            case 2: {
                bool isLogin = false;
                if (loginUser(isLogin)) {
                    if (isLogin) menuAdmin();
                    else menuUser();
                } else {
                    cout << "Login gagal setelah 3 kali percobaan.\n";
                    system("pause");
                }
                break;
            }
            case 3: running = false; break;
            default: cout << "Pilihan tidak valid!\n"; system("pause");
        }
    }
    return 0;
}
