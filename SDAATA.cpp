#include <iostream>
#include <string>
using namespace std;

struct Barang {
    string id;
    string nama;
    int jumlah;
    bool prioritas; // Menandai barang dengan prioritas tinggi
    string status;  // Menyimpan status barang (untuk penerimaan)
};

struct RiwayatKerusakan {
    string idBarang;
    string deskripsi;
};

// Data akan disimpan dalam array statis
Barang inventaris[100];
int inventarisCount = 0;

Barang fifoQueue[100];
int fifoFront = 0, fifoBack = 0;

Barang prioritasQueue[100];  // Antrian khusus untuk barang prioritas
int prioritasFront = 0, prioritasBack = 0;

Barang penerimaanQueue[100];  // Antrian penerimaan barang
int penerimaanFront = 0, penerimaanBack = 0;  // Menambahkan penerimaanFront dan penerimaanBack

RiwayatKerusakan riwayatKerusakan[100];
int riwayatCount = 0;

// Fungsi untuk menambahkan barang ke inventaris dan antrian FIFO atau Prioritas
void tambahBarang(const string& id, const string& nama, int jumlah, bool prioritas) {
    Barang barang = {id, nama, jumlah, prioritas, "Tersedia"};
    inventaris[inventarisCount++] = barang;
    cout << "Barang " << nama << " (ID: " << id << ") berhasil ditambahkan ke inventaris.\n";

    // Menambahkan barang ke antrian FIFO
    fifoQueue[fifoBack++] = barang;

    // Jika barang prioritas, tambahkan ke antrian prioritas
    if (prioritas) {
        prioritasQueue[prioritasBack++] = barang;
    }
}

// Fungsi untuk mencatat kerusakan barang
void catatKerusakan(const string& id, const string& deskripsi) {
    int index = -1;
    for (int i = 0; i < inventarisCount; ++i) {
        if (inventaris[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Barang dengan ID " << id << " tidak ditemukan di inventaris.\n";
        return;
    }

    riwayatKerusakan[riwayatCount++] = {id, deskripsi};

    for (int i = index; i < inventarisCount - 1; ++i) {
        inventaris[i] = inventaris[i + 1];
    }
    --inventarisCount;

    cout << "Kerusakan pada barang dengan ID " << id << " berhasil dicatat dan dihapus dari inventaris.\n";
}

// Fungsi untuk proses barang keluar (FIFO)
void prosesKeluarBarang() {
    if (fifoFront == fifoBack) {
        cout << "Tidak ada barang untuk diproses keluar.\n";
        return;
    }

    Barang barang = fifoQueue[fifoFront++];
    cout << "Barang keluar: " << barang.nama << " (ID: " << barang.id << ").\n";

    // Menghapus barang dari inventaris
    for (int i = 0; i < inventarisCount; ++i) {
        if (inventaris[i].id == barang.id) {
            for (int j = i; j < inventarisCount - 1; ++j) {
                inventaris[j] = inventaris[j + 1];
            }
            --inventarisCount;
            break;
        }
    }
}

// Fungsi untuk proses pengiriman prioritas (FIFO untuk prioritas)
void prosesPrioritas() {
    if (prioritasFront == prioritasBack) {
        cout << "Tidak ada barang prioritas untuk diproses.\n";
        return;
    }

    Barang barang = prioritasQueue[prioritasFront++];
    cout << "Pengiriman prioritas: " << barang.nama << " (ID: " << barang.id << ").\n";

    // Menghapus barang dari inventaris
    for (int i = 0; i < inventarisCount; ++i) {
        if (inventaris[i].id == barang.id) {
            for (int j = i; j < inventarisCount - 1; ++j) {
                inventaris[j] = inventaris[j + 1];
            }
            --inventarisCount;
            break;
        }
    }
}

// Fungsi untuk menambahkan barang ke antrian penerimaan
void tambahPenerimaanBarang(const string& id, const string& nama, int jumlah, bool prioritas) {
    if (penerimaanBack >= 100) {
        cout << "Antrian penerimaan penuh.\n";
        return;
    }
    Barang barang = {id, nama, jumlah, prioritas, "Menunggu diperiksa"};
    penerimaanQueue[penerimaanBack++] = barang;
    cout << "Barang " << nama << " (ID: " << id << ") berhasil ditambahkan ke antrian penerimaan dengan status 'Menunggu diperiksa'.\n";
}

// Fungsi untuk menampilkan inventaris
void tampilkanInventaris() {
    cout << "\nDaftar Barang di Inventaris:\n";
    if (inventarisCount == 0) {
        cout << "Tidak ada barang di inventaris.\n";
        return;
    }

    for (int i = 0; i < inventarisCount; ++i) {
        cout << "ID: " << inventaris[i].id << ", Nama: " << inventaris[i].nama
             << ", Jumlah: " << inventaris[i].jumlah
             << ", Prioritas: " << (inventaris[i].prioritas ? "Ya" : "Tidak") << "\n";
    }
}

// Fungsi untuk menampilkan riwayat kerusakan
void tampilkanRiwayatKerusakan() {
    cout << "\nRiwayat Kerusakan atau Kehilangan:\n";
    if (riwayatCount == 0) {
        cout << "Tidak ada riwayat kerusakan atau kehilangan.\n";
        return;
    }

    for (int i = 0; i < riwayatCount; ++i) {
        cout << "ID Barang: " << riwayatKerusakan[i].idBarang
             << ", Deskripsi: " << riwayatKerusakan[i].deskripsi << "\n";
    }
}

// Fungsi untuk menampilkan antrian penerimaan
void tampilkanPenerimaanBarang() {
    cout << "\nStatus Antrian Penerimaan Barang:\n";
    if (penerimaanFront == penerimaanBack) {
        cout << "Tidak ada barang dalam antrian penerimaan.\n";
        return;
    }

    for (int i = penerimaanFront; i < penerimaanBack; ++i) {
        cout << "ID: " << penerimaanQueue[i].id << ", Nama: " << penerimaanQueue[i].nama
             << ", Jumlah: " << penerimaanQueue[i].jumlah
             << ", Prioritas: " << (penerimaanQueue[i].prioritas ? "Ya" : "Tidak")
             << ", Status: " << penerimaanQueue[i].status << "\n";
    }
}

// Fungsi untuk memproses barang dari antrian penerimaan
void prosesPenerimaanBarang() {
    if (penerimaanFront == penerimaanBack) {
        cout << "Tidak ada barang dalam antrian penerimaan untuk diproses.\n";
        return;
    }

    Barang& barang = penerimaanQueue[penerimaanFront];
    cout << "Proses barang: " << barang.nama << " (ID: " << barang.id << ")\n";
    cout << "1. Terverifikasi\n2. Tidak Terverifikasi\n";
    cout << "Masukkan pilihan (1/2): ";
    int pilihan;
    cin >> pilihan;

    if (pilihan == 1) {
        cout << "Barang " << barang.nama << " terverifikasi.\n";
        tambahBarang(barang.id, barang.nama, barang.jumlah, barang.prioritas);
    } else if (pilihan == 2) {
        cout << "Barang " << barang.nama << " tidak terverifikasi dan dihapus dari antrian.\n";
    } else {
        cout << "Pilihan tidak valid. Barang tetap di antrian.\n";
        return;
    }

    // Hapus barang dari antrian penerimaan
    for (int i = penerimaanFront; i < penerimaanBack - 1; ++i) {
        penerimaanQueue[i] = penerimaanQueue[i + 1];
    }
    --penerimaanBack;
}

// Fungsi utama
int main() {
    int pilihan;
    do {
        cout << "\n=== Menu Manajemen Inventaris ===\n";
        cout << "1. Tambah Penerimaan Barang\n";
        cout << "2. Tampilkan Penerimaan Barang\n";
        cout << "3. Proses Penerimaan Barang\n";
        cout << "4. Tampilkan Inventaris\n";
        cout << "5. Proses Barang Keluar\n";
        cout << "6. Proses Pengiriman Prioritas\n";
        cout << "7. Catat Kerusakan/Hilang Barang\n";
        cout << "8. Tampilkan Riwayat Kerusakan\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
        	case 1: {
                string id, nama;
                int jumlah;
                bool prioritas;
                cout << "Masukkan ID Barang: ";
                cin >> id;
                cout << "Masukkan Nama Barang: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Masukkan Jumlah Barang: ";
                cin >> jumlah;
                cout << "Apakah barang ini prioritas? (1 = Ya, 0 = Tidak): ";
                cin >> prioritas;
                tambahPenerimaanBarang(id, nama, jumlah, prioritas);
                break;
            }
            case 2:
                tampilkanPenerimaanBarang();
                break;
            case 3:
                prosesPenerimaanBarang();
                break;
            case 4:
                tampilkanInventaris();
                break;
            case 5:
                prosesKeluarBarang();
                break;
            case 6:
                prosesPrioritas();
                break;
            case 7: {
                string id, deskripsi;
                cout << "Masukkan ID Barang: ";
                cin >> id;
                cout << "Masukkan deskripsi kerusakan: ";
                cin.ignore();
                getline(cin, deskripsi);
                catatKerusakan(id, deskripsi);
                break;
            }
            case 8:
                tampilkanRiwayatKerusakan();
                break;
            case 0:
                cout << "Terima kasih! Program selesai.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}

