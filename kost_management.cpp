/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║        SISTEM MANAJEMEN KOST/KONTRAKAN - C++                ║
 * ║  Lantai 1-2 : Kamar REGULER                                 ║
 * ║  Lantai 3-4 : Kamar VIP                                     ║
 * ║  9 Struktur Data: Array, Linked List, Stack, Queue,         ║
 * ║                   BST, Graph+BFS, Sorting, Searching        ║
 * ╚══════════════════════════════════════════════════════════════╝
 */

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cmath>
using namespace std;

// ═══════════════════════════════════════════════════════
//  KONSTANTA & ENUM
// ═══════════════════════════════════════════════════════
const int MAX_KAMAR = 40;
const int MAX_NODES = 12;

enum TipeKamar   { REGULAR_NONAC, REGULAR_AC, VIP_NONAC, VIP_AC };
enum StatusKamar { KOSONG, DIHUNI, PERBAIKAN };

string tipeStr(TipeKamar t) {
    switch(t) {
        case REGULAR_NONAC: return "Regular Non-AC";
        case REGULAR_AC:    return "Regular AC";
        case VIP_NONAC:     return "VIP Non-AC";
        case VIP_AC:        return "VIP AC";
    }
    return "-";
}
string tipeStrPendek(TipeKamar t) {
    switch(t) {
        case REGULAR_NONAC: return "Reg Non-AC";
        case REGULAR_AC:    return "Reg AC";
        case VIP_NONAC:     return "VIP Non-AC";
        case VIP_AC:        return "VIP AC";
    }
    return "-";
}
string statusStr(StatusKamar s) {
    if (s == KOSONG)    return "Kosong";
    if (s == DIHUNI)    return "Dihuni";
    return "Perbaikan";
}
int getLantai(int nomorKamar) { return nomorKamar / 100; }

// ═══════════════════════════════════════════════════════
// 1. ARRAY + POINTER — Daftar Kamar
// ═══════════════════════════════════════════════════════
struct Kamar {
    int          nomorKamar;
    int          lantai;
    TipeKamar    tipe;
    StatusKamar  status;
    double       sewaBulanan;
    double       deposit;        // 2x sewa
    string       namaPenghuni;
    string       fasilitasExtra; // fasilitas bonus VIP
};

Kamar* daftarKamar[MAX_KAMAR];
int    totalKamar = 0;

void inisialisasiKamar() {
    int idx = 0;
    // Lantai 1 — Regular Non-AC (10 kamar: 101-110), Rp800.000
    for (int i = 1; i <= 10; i++) {
        daftarKamar[idx] = new Kamar();
        daftarKamar[idx]->nomorKamar    = 100 + i;
        daftarKamar[idx]->lantai        = 1;
        daftarKamar[idx]->tipe          = REGULAR_NONAC;
        daftarKamar[idx]->status        = KOSONG;
        daftarKamar[idx]->sewaBulanan   = 800000;
        daftarKamar[idx]->deposit       = 1600000;
        daftarKamar[idx]->namaPenghuni  = "";
        daftarKamar[idx]->fasilitasExtra= "Kipas angin, Lemari, Kasur";
        idx++;
    }
    // Lantai 2 — Regular AC (10 kamar: 201-210), Rp1.100.000
    for (int i = 1; i <= 10; i++) {
        daftarKamar[idx] = new Kamar();
        daftarKamar[idx]->nomorKamar    = 200 + i;
        daftarKamar[idx]->lantai        = 2;
        daftarKamar[idx]->tipe          = REGULAR_AC;
        daftarKamar[idx]->status        = KOSONG;
        daftarKamar[idx]->sewaBulanan   = 1100000;
        daftarKamar[idx]->deposit       = 2200000;
        daftarKamar[idx]->namaPenghuni  = "";
        daftarKamar[idx]->fasilitasExtra= "AC, Lemari, Kasur, Meja Belajar";
        idx++;
    }
    // Lantai 3 — VIP Non-AC (10 kamar: 301-310), Rp1.500.000
    for (int i = 1; i <= 10; i++) {
        daftarKamar[idx] = new Kamar();
        daftarKamar[idx]->nomorKamar    = 300 + i;
        daftarKamar[idx]->lantai        = 3;
        daftarKamar[idx]->tipe          = VIP_NONAC;
        daftarKamar[idx]->status        = KOSONG;
        daftarKamar[idx]->sewaBulanan   = 1500000;
        daftarKamar[idx]->deposit       = 3000000;
        daftarKamar[idx]->namaPenghuni  = "";
        daftarKamar[idx]->fasilitasExtra= "Kipas angin, Lemari, Kasur Spring Bed, TV, Kamar Mandi Dalam";
        idx++;
    }
    // Lantai 4 — VIP AC (10 kamar: 401-410), Rp2.000.000
    for (int i = 1; i <= 10; i++) {
        daftarKamar[idx] = new Kamar();
        daftarKamar[idx]->nomorKamar    = 400 + i;
        daftarKamar[idx]->lantai        = 4;
        daftarKamar[idx]->tipe          = VIP_AC;
        daftarKamar[idx]->status        = KOSONG;
        daftarKamar[idx]->sewaBulanan   = 2000000;
        daftarKamar[idx]->deposit       = 4000000;
        daftarKamar[idx]->namaPenghuni  = "";
        daftarKamar[idx]->fasilitasExtra= "AC, Lemari, Kasur Spring Bed, TV, Kamar Mandi Dalam, Kulkas Mini";
        idx++;
    }
    totalKamar = idx;
}

// ═══════════════════════════════════════════════════════
// 2. LINKED LIST — Penghuni Aktif & Riwayat Transaksi
// ═══════════════════════════════════════════════════════
struct NodePenghuni {
    string        nama;
    string        noKTP;
    string        noHP;
    string        pekerjaan;
    int           nomorKamar;
    int           durasiKontrak;
    string        tanggalMasuk;
    NodePenghuni* next;
};

struct Transaksi {
    int    id;
    int    nomorKamar;
    string namaPenghuni;
    string jenis;
    double jumlah;
    string tanggal;
};
struct NodeTransaksi {
    Transaksi      data;
    NodeTransaksi* next;
};

NodePenghuni*  headPenghuni  = nullptr;
int            totalPenghuni = 0;
NodeTransaksi* headTransaksi = nullptr;
int            idTransNext   = 1001;

void tambahPenghuni(const string& nama, const string& ktp,
                    const string& hp,   const string& pekerjaan,
                    int kamar, int durasi, const string& tgl) {
    NodePenghuni* baru   = new NodePenghuni();
    baru->nama           = nama;
    baru->noKTP          = ktp;
    baru->noHP           = hp;
    baru->pekerjaan      = pekerjaan;
    baru->nomorKamar     = kamar;
    baru->durasiKontrak  = durasi;
    baru->tanggalMasuk   = tgl;
    baru->next           = headPenghuni;
    headPenghuni         = baru;
    totalPenghuni++;
}

bool hapusPenghuni(int nomorKamar) {
    NodePenghuni *curr = headPenghuni, *prev = nullptr;
    while (curr) {
        if (curr->nomorKamar == nomorKamar) {
            if (prev) prev->next   = curr->next;
            else      headPenghuni = curr->next;
            delete curr;
            totalPenghuni--;
            return true;
        }
        prev = curr; curr = curr->next;
    }
    return false;
}

void catatTransaksi(int kamar, const string& nama,
                    const string& jenis, double jumlah, const string& tgl) {
    NodeTransaksi* baru    = new NodeTransaksi();
    baru->data.id          = idTransNext++;
    baru->data.nomorKamar  = kamar;
    baru->data.namaPenghuni= nama;
    baru->data.jenis       = jenis;
    baru->data.jumlah      = jumlah;
    baru->data.tanggal     = tgl;
    baru->next             = headTransaksi;
    headTransaksi          = baru;
}

// ═══════════════════════════════════════════════════════
// 3. STACK — Log Aktivitas & Undo
// ═══════════════════════════════════════════════════════
struct AksiLog { string tipe; int kamar; string ket; };
stack<AksiLog> logAksi;
stack<AksiLog> undoStack;

void pushLog(const string& tipe, int kamar, const string& ket) {
    AksiLog a = {tipe, kamar, ket};
    logAksi.push(a);
    undoStack.push(a);
}

// ═══════════════════════════════════════════════════════
// 4. QUEUE — Antrian Tagihan & Laporan Perbaikan
// ═══════════════════════════════════════════════════════
struct TagihanSewa { int kamar; string nama; double sewa; double listrik; string bulan; };
struct LaporPerbaikan { int kamar; string keluhan; string prioritas; };

queue<TagihanSewa>   antrianTagihan;
queue<LaporPerbaikan> antrianPerbaikan;

void tambahTagihan(int kamar, const string& nama,
                   double sewa, double listrik, const string& bulan) {
    antrianTagihan.push({kamar, nama, sewa, listrik, bulan});
}

// ═══════════════════════════════════════════════════════
// 5. BST — Cari Penghuni Cepat
// ═══════════════════════════════════════════════════════
struct NodeBST { string nama; int kamar; NodeBST *kiri, *kanan; };
NodeBST* rootBST = nullptr;

NodeBST* insertBST(NodeBST* r, const string& nama, int kamar) {
    if (!r) {
        NodeBST* b = new NodeBST(); b->nama=nama; b->kamar=kamar; b->kiri=b->kanan=nullptr; return b;
    }
    if (nama < r->nama) r->kiri  = insertBST(r->kiri,  nama, kamar);
    else                r->kanan = insertBST(r->kanan, nama, kamar);
    return r;
}
NodeBST* cariBST(NodeBST* r, const string& nama) {
    if (!r || r->nama == nama) return r;
    return nama < r->nama ? cariBST(r->kiri, nama) : cariBST(r->kanan, nama);
}
NodeBST* hapusBST(NodeBST* r, const string& nama) {
    if (!r) return nullptr;
    if (nama < r->nama)      r->kiri  = hapusBST(r->kiri,  nama);
    else if (nama > r->nama) r->kanan = hapusBST(r->kanan, nama);
    else {
        if (!r->kiri)  { NodeBST* t=r->kanan; delete r; return t; }
        if (!r->kanan) { NodeBST* t=r->kiri;  delete r; return t; }
        NodeBST* s=r->kanan; while(s->kiri) s=s->kiri;
        r->nama=s->nama; r->kamar=s->kamar;
        r->kanan=hapusBST(r->kanan, s->nama);
    }
    return r;
}
void inOrderBST(NodeBST* r) {
    if (!r) return;
    inOrderBST(r->kiri);
    cout << "  " << left << setw(26) << r->nama << "Kamar " << r->kamar << "\n";
    inOrderBST(r->kanan);
}

// ═══════════════════════════════════════════════════════
// 6. GRAPH + BFS — Denah Lantai & Navigasi
// ═══════════════════════════════════════════════════════
// 0=Pintu Masuk, 1=Parkir Motor, 2=Parkir Mobil,
// 3=Lantai 1 (Regular), 4=Lantai 2 (Regular),
// 5=Lantai 3 (VIP), 6=Lantai 4 (VIP),
// 7=Dapur Bersama, 8=KM Umum Lt1-2, 9=Ruang Jemur,
// 10=Mushola, 11=Warung/Minimarket
string namaNode[MAX_NODES] = {
    "Pintu Masuk","Parkir Motor","Parkir Mobil",
    "Lantai 1 (Regular)","Lantai 2 (Regular)",
    "Lantai 3 (VIP)","Lantai 4 (VIP)",
    "Dapur Bersama","KM Umum Lt1-2","Ruang Jemur",
    "Mushola","Warung/Minimarket"
};
bool adj[MAX_NODES][MAX_NODES] = {};

void bangunGraph() {
    // Pintu masuk
    adj[0][1]=adj[1][0]=true; // parkir motor
    adj[0][2]=adj[2][0]=true; // parkir mobil
    adj[0][3]=adj[3][0]=true; // lantai 1
    adj[0][11]=adj[11][0]=true; // warung
    // Antar lantai (tangga/lift)
    adj[3][4]=adj[4][3]=true;
    adj[4][5]=adj[5][4]=true;
    adj[5][6]=adj[6][5]=true;
    // Fasilitas dari lantai 1
    adj[3][7]=adj[7][3]=true;  // dapur
    adj[3][8]=adj[8][3]=true;  // km umum
    adj[3][9]=adj[9][3]=true;  // jemur
    adj[3][10]=adj[10][3]=true; // mushola
}

vector<int> bfsCari(int mulai, int tujuan) {
    vector<bool> vis(MAX_NODES,false);
    vector<int> par(MAX_NODES,-1);
    queue<int> q;
    q.push(mulai); vis[mulai]=true;
    while (!q.empty()) {
        int c=q.front(); q.pop();
        if (c==tujuan) break;
        for (int i=0;i<MAX_NODES;i++)
            if (adj[c][i]&&!vis[i]) { vis[i]=true; par[i]=c; q.push(i); }
    }
    vector<int> jalur;
    if (!vis[tujuan]) return jalur;
    for (int v=tujuan;v!=-1;v=par[v]) jalur.push_back(v);
    reverse(jalur.begin(),jalur.end());
    return jalur;
}

// ═══════════════════════════════════════════════════════
// 7. SORTING — Urutkan Kamar Berdasarkan Harga Sewa
// ═══════════════════════════════════════════════════════
void bubbleSort(vector<Kamar*>& arr, bool asc=true) {
    int n=arr.size();
    for (int i=0;i<n-1;i++)
        for (int j=0;j<n-i-1;j++) {
            bool tukar = asc ? arr[j]->sewaBulanan > arr[j+1]->sewaBulanan
                             : arr[j]->sewaBulanan < arr[j+1]->sewaBulanan;
            if (tukar) swap(arr[j],arr[j+1]);
        }
}

// ═══════════════════════════════════════════════════════
// 8. SEARCHING — Cari Kamar & Penghuni
// ═══════════════════════════════════════════════════════
Kamar* cariKamar(int nomor) {
    for (int i=0;i<totalKamar;i++)
        if (daftarKamar[i]->nomorKamar==nomor) return daftarKamar[i];
    return nullptr;
}
vector<Kamar*> kamarKosongByTipe(TipeKamar tipe=(TipeKamar)-1) {
    vector<Kamar*> hasil;
    for (int i=0;i<totalKamar;i++) {
        bool cocok=(tipe==(TipeKamar)-1)||(daftarKamar[i]->tipe==tipe);
        if (daftarKamar[i]->status==KOSONG && cocok) hasil.push_back(daftarKamar[i]);
    }
    return hasil;
}
NodePenghuni* cariPenghuni(const string& nama) {
    NodePenghuni* c=headPenghuni;
    while (c) { if (c->nama==nama) return c; c=c->next; }
    return nullptr;
}

// ═══════════════════════════════════════════════════════
//  HELPER INPUT
// ═══════════════════════════════════════════════════════
void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
string inputString(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}
int inputInt(const string& prompt) {
    int n;
    while (true) {
        cout << prompt;
        if (cin >> n) { clearInput(); return n; }
        cout << "  [!] Masukkan angka yang valid!\n";
        cin.clear(); clearInput();
    }
}
double inputDouble(const string& prompt) {
    double n;
    while (true) {
        cout << prompt;
        if (cin >> n) { clearInput(); return n; }
        cout << "  [!] Masukkan angka yang valid!\n";
        cin.clear(); clearInput();
    }
}

// ═══════════════════════════════════════════════════════
//  FORMAT RUPIAH (titik ribuan)
// ═══════════════════════════════════════════════════════
string formatRupiah(double angka) {
    long long n = (long long) llround(angka);
    bool negatif = n < 0;
    if (negatif) n = -n;
    string s = to_string(n);
    string hasil;
    int hitung = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        hasil = s[i] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i != 0) hasil = "." + hasil;
    }
    return (negatif ? "-" : "") + hasil;
}

// ═══════════════════════════════════════════════════════
//  TAMPILAN
// ═══════════════════════════════════════════════════════
void garisH(int n=65, char c='-') { cout << "  "; for(int i=0;i<n;i++) cout<<c; cout<<"\n"; }

void tampilkanSemuaKamar() {
    cout << "\n+===============================================================+\n";
    cout << "|                    DAFTAR SEMUA KAMAR                        |\n";
    cout << "+===============================================================+\n";
    // Per lantai
    for (int lantai=1;lantai<=4;lantai++) {
        string label = (lantai<=2) ? "REGULER" : "VIP";
        cout << "\n  [ LANTAI " << lantai << " - " << label << " ]\n";
        garisH(62);
        cout << "  " << left << setw(8)<<"No Kamar" << setw(16)<<"Tipe"
             << setw(12)<<"Status" << setw(22)<<"Penghuni" << "Sewa/Bln\n";
        garisH(62);
        for (int i=0;i<totalKamar;i++) {
            Kamar* k=daftarKamar[i];
            if (k->lantai!=lantai) continue;
            cout << "  " << setw(8) << k->nomorKamar
                 << setw(16) << tipeStrPendek(k->tipe)
                 << setw(12) << statusStr(k->status)
                 << setw(22) << (k->namaPenghuni.empty()?"-":k->namaPenghuni)
                 << "Rp" << formatRupiah(k->sewaBulanan) << "\n";
        }
    }
    cout << "\n";
}

void tampilkanKamarKosong() {
    cout << "\n+===============================================================+\n";
    cout << "|                   KAMAR TERSEDIA / KOSONG                    |\n";
    cout << "+===============================================================+\n";
    for (int lantai=1;lantai<=4;lantai++) {
        string label=(lantai<=2)?"REGULER":"VIP";
        cout << "\n  [ LANTAI " << lantai << " - " << label << " ]\n";
        garisH(55);
        cout << "  " << left << setw(10)<<"No Kamar" << setw(16)<<"Tipe"
             << setw(16)<<"Sewa/Bln" << "Deposit\n";
        garisH(55);
        bool ada=false;
        for (int i=0;i<totalKamar;i++) {
            Kamar* k=daftarKamar[i];
            if (k->lantai!=lantai || k->status!=KOSONG) continue;
            cout << "  " << setw(10) << k->nomorKamar
                 << setw(16) << tipeStrPendek(k->tipe)
                 << "Rp" << setw(14) << formatRupiah(k->sewaBulanan)
                 << "Rp" << formatRupiah(k->deposit) << "\n";
            ada=true;
        }
        if (!ada) cout << "  (semua kamar di lantai ini penuh)\n";
    }
    cout << "\n";
}

void tampilkanDaftarPenghuni() {
    cout << "\n+===============================================================+\n";
    cout << "|                   DAFTAR PENGHUNI AKTIF                      |\n";
    cout << "+===============================================================+\n";
    cout << "  Total penghuni aktif: " << totalPenghuni << "\n\n";
    garisH(70);
    cout << "  " << left << setw(24)<<"Nama" << setw(8)<<"Kamar"
         << setw(14)<<"Tipe Kamar" << setw(8)<<"Durasi" << setw(14)<<"Tgl Masuk" << "No HP\n";
    garisH(70);
    NodePenghuni* curr=headPenghuni;
    if (!curr) cout << "  (belum ada penghuni)\n";
    while (curr) {
        Kamar* k=cariKamar(curr->nomorKamar);
        string tipe = k ? tipeStrPendek(k->tipe) : "-";
        cout << "  " << setw(24) << curr->nama
             << setw(8)  << curr->nomorKamar
             << setw(14) << tipe
             << setw(8)  << (to_string(curr->durasiKontrak)+" bln")
             << setw(14) << curr->tanggalMasuk
             << curr->noHP << "\n";
        curr=curr->next;
    }
    cout << "\n";
}

void tampilkanRiwayatTransaksi() {
    cout << "\n+===============================================================+\n";
    cout << "|                    RIWAYAT TRANSAKSI                         |\n";
    cout << "+===============================================================+\n";
    cout << "  " << left << setw(7)<<"ID" << setw(7)<<"Kamar"
         << setw(22)<<"Penghuni" << setw(18)<<"Jenis"
         << setw(14)<<"Jumlah" << "Tanggal\n";
    garisH(72);
    NodeTransaksi* curr=headTransaksi;
    if (!curr) cout << "  (belum ada transaksi)\n";
    while (curr) {
        cout << "  " << setw(7) << curr->data.id
             << setw(7) << curr->data.nomorKamar
             << setw(22) << curr->data.namaPenghuni
             << setw(18) << curr->data.jenis
             << "Rp" << setw(12) << formatRupiah(curr->data.jumlah)
             << curr->data.tanggal << "\n";
        curr=curr->next;
    }
    cout << "\n";
}

void tampilkanLog() {
    stack<AksiLog> tmp=logAksi;
    cout << "\n+===============================================================+\n";
    cout << "|              LOG AKTIVITAS (Terbaru di Atas)                 |\n";
    cout << "+===============================================================+\n";
    if (tmp.empty()) cout << "  (belum ada aktivitas)\n";
    while (!tmp.empty()) {
        auto& a=tmp.top();
        cout << "  [" << left << setw(12) << a.tipe << "] Kamar " << a.kamar
             << " - " << a.ket << "\n";
        tmp.pop();
    }
    cout << "\n";
}

void tampilkanAntrianTagihan() {
    queue<TagihanSewa> tmp=antrianTagihan;
    cout << "\n+===============================================================+\n";
    cout << "|                    ANTRIAN TAGIHAN SEWA                      |\n";
    cout << "+===============================================================+\n";
    int no=1;
    if (tmp.empty()) cout << "  (antrian tagihan kosong)\n";
    while (!tmp.empty()) {
        auto& t=tmp.front();
        double total=t.sewa+t.listrik;
        cout << "  " << no++ << ". Kamar " << t.kamar
             << " | " << left << setw(20) << t.nama
             << t.bulan << " | Total: Rp"
             << formatRupiah(total) << "\n";
        tmp.pop();
    }
    cout << "\n";
}

void tampilkanAntrianPerbaikan() {
    queue<LaporPerbaikan> tmp=antrianPerbaikan;
    cout << "\n+===============================================================+\n";
    cout << "|                  ANTRIAN LAPORAN PERBAIKAN                   |\n";
    cout << "+===============================================================+\n";
    int no=1;
    if (tmp.empty()) cout << "  (tidak ada laporan perbaikan)\n";
    while (!tmp.empty()) {
        auto& p=tmp.front();
        cout << "  " << no++ << ". Kamar " << p.kamar
             << " [" << p.prioritas << "] " << p.keluhan << "\n";
        tmp.pop();
    }
    cout << "\n";
}

void tampilkanDenah() {
    cout << "\n+===============================================================+\n";
    cout << "|               DENAH & FASILITAS KOST                         |\n";
    cout << "+===============================================================+\n";
    for (int i=0;i<MAX_NODES;i++) {
        cout << "  [" << i << "] " << left << setw(22) << namaNode[i] << "-> ";
        bool ada=false;
        for (int j=0;j<MAX_NODES;j++)
            if (adj[i][j]) { cout << namaNode[j] << "  "; ada=true; }
        if (!ada) cout << "-";
        cout << "\n";
    }
    cout << "\n";
}

void tampilkanSortedHarga(bool asc) {
    vector<Kamar*> tmp(daftarKamar, daftarKamar+totalKamar);
    bubbleSort(tmp, asc);
    cout << "\n+===============================================================+\n";
    cout << "|  KAMAR DIURUTKAN HARGA " << (asc?"TERMURAH":"TERMAHAL")
         << "                              |\n";
    cout << "+===============================================================+\n";
    cout << "  " << left << setw(10)<<"No Kamar" << setw(8)<<"Lantai"
         << setw(16)<<"Tipe" << setw(12)<<"Status" << "Sewa/Bln\n";
    garisH(56);
    for (auto* k:tmp)
        cout << "  " << setw(10) << k->nomorKamar
             << setw(8)  << k->lantai
             << setw(16) << tipeStrPendek(k->tipe)
             << setw(12) << statusStr(k->status)
             << "Rp" << formatRupiah(k->sewaBulanan) << "\n";
    cout << "\n";
}

// ═══════════════════════════════════════════════════════
//  AKSI UTAMA
// ═══════════════════════════════════════════════════════
void penghunIMasuk() {
    cout << "\n+--- INPUT DATA PENGHUNI BARU ----------------------------------+\n";
    tampilkanKamarKosong();

    // Input data penghuni
    string nama       = inputString("  Nama Lengkap       : ");
    string ktp        = inputString("  No KTP (16 digit)  : ");
    string hp         = inputString("  No HP              : ");
    string pekerjaan  = inputString("  Pekerjaan          : ");
    int    durasi     = inputInt   ("  Durasi Kontrak (bln): ");
    string tglMasuk   = inputString("  Tanggal Masuk (cth: 2026-06-03): ");

    // Pilih kamar
    cout << "\n  Pilihan Tipe Kamar:\n";
    cout << "  [1] Regular Non-AC (Lantai 1) - Rp 800.000/bln\n";
    cout << "  [2] Regular AC     (Lantai 2) - Rp 1.100.000/bln\n";
    cout << "  [3] VIP Non-AC     (Lantai 3) - Rp 1.500.000/bln\n";
    cout << "  [4] VIP AC         (Lantai 4) - Rp 2.000.000/bln\n";
    int pilihanTipe = inputInt("  Pilih tipe kamar [1-4]: ");

    TipeKamar tipeTarget;
    switch (pilihanTipe) {
        case 1: tipeTarget = REGULAR_NONAC; break;
        case 2: tipeTarget = REGULAR_AC;    break;
        case 3: tipeTarget = VIP_NONAC;     break;
        case 4: tipeTarget = VIP_AC;        break;
        default:
            cout << "  [!] Pilihan tidak valid.\n"; return;
    }

    auto kamarKosong = kamarKosongByTipe(tipeTarget);
    if (kamarKosong.empty()) {
        cout << "  [!] Tidak ada kamar kosong untuk tipe tersebut!\n"; return;
    }

    cout << "\n  Kamar tersedia tipe ini:\n";
    for (auto* k : kamarKosong)
        cout << "    - Kamar " << k->nomorKamar << " (Lantai " << k->lantai << ")\n";

    int nomorKamar = inputInt("  Pilih Nomor Kamar  : ");
    Kamar* k = cariKamar(nomorKamar);
    if (!k || k->status != KOSONG || k->tipe != tipeTarget) {
        cout << "  [!] Nomor kamar tidak valid atau tidak tersedia!\n"; return;
    }

    // Proses masuk
    k->status       = DIHUNI;
    k->namaPenghuni = nama;
    tambahPenghuni(nama, ktp, hp, pekerjaan, nomorKamar, durasi, tglMasuk);
    rootBST = insertBST(rootBST, nama, nomorKamar);

    double totalBayarAwal = k->deposit + k->sewaBulanan;
    catatTransaksi(nomorKamar, nama, "MASUK+DEPOSIT", totalBayarAwal, tglMasuk);
    tambahTagihan(nomorKamar, nama, k->sewaBulanan, 0, "Bulan-1");
    pushLog("MASUK", nomorKamar, nama + " | Kontrak " + to_string(durasi) + " bln");

    cout << "\n  +----------------------------------------------------------+\n";
    cout << "  | PENGHUNI BERHASIL DIDAFTARKAN!                           |\n";
    cout << "  +----------------------------------------------------------+\n";
    cout << "  Nama        : " << nama << "\n";
    cout << "  No KTP      : " << ktp << "\n";
    cout << "  No HP       : " << hp << "\n";
    cout << "  Pekerjaan   : " << pekerjaan << "\n";
    cout << "  Kamar       : " << nomorKamar << " - " << tipeStr(k->tipe) << " (Lantai " << k->lantai << ")\n";
    cout << "  Fasilitas   : " << k->fasilitasExtra << "\n";
    cout << "  Durasi      : " << durasi << " bulan\n";
    cout << "  Tgl Masuk   : " << tglMasuk << "\n";
    cout << "  Sewa/Bln    : Rp" << formatRupiah(k->sewaBulanan) << "\n";
    cout << "  Deposit     : Rp" << formatRupiah(k->deposit) << "\n";
    cout << "  Bayar Awal  : Rp" << formatRupiah(totalBayarAwal) << " (deposit + sewa bln 1)\n";
    cout << "  +----------------------------------------------------------+\n";
}

void penghunIKeluar() {
    cout << "\n+--- PROSES PENGHUNI KELUAR -----------------------------------+\n";
    tampilkanDaftarPenghuni();

    int nomorKamar  = inputInt   ("  Nomor Kamar Penghuni yang Keluar: ");
    string tglKeluar= inputString("  Tanggal Keluar (cth: 2026-06-03) : ");

    Kamar* k = cariKamar(nomorKamar);
    if (!k || k->status != DIHUNI) {
        cout << "  [!] Kamar tidak sedang dihuni!\n"; return;
    }

    string nama = k->namaPenghuni;

    // Cek tagihan belum lunas
    cout << "\n  Konfirmasi keluar untuk: " << nama << " (Kamar " << nomorKamar << ")\n";
    cout << "  Deposit dikembalikan: Rp" << formatRupiah(k->deposit) << "\n";
    cout << "  Lanjut? [1=Ya / 0=Tidak]: ";
    int konfirm; cin >> konfirm; clearInput();
    if (konfirm != 1) { cout << "  Dibatalkan.\n"; return; }

    catatTransaksi(nomorKamar, nama, "KELUAR", k->deposit, tglKeluar);
    k->status       = KOSONG;
    k->namaPenghuni = "";
    rootBST = hapusBST(rootBST, nama);
    hapusPenghuni(nomorKamar);
    pushLog("KELUAR", nomorKamar, nama + " | Deposit kembali | " + tglKeluar);

    cout << "  [OK] Penghuni " << nama << " berhasil keluar dari Kamar "
         << nomorKamar << ".\n";
    cout << "  Deposit Rp" << formatRupiah(k->deposit) << " dikembalikan.\n";
}

void bayarSewa() {
    cout << "\n+--- PEMBAYARAN SEWA BULANAN ----------------------------------+\n";
    tampilkanDaftarPenghuni();

    int nomorKamar    = inputInt   ("  Nomor Kamar       : ");
    string bulan      = inputString("  Bulan Pembayaran  : ");
    string tgl        = inputString("  Tanggal Bayar     : ");
    double listrik    = inputDouble("  Tagihan Listrik (Rp): ");

    Kamar* k = cariKamar(nomorKamar);
    if (!k || k->status != DIHUNI) {
        cout << "  [!] Kamar tidak dihuni!\n"; return;
    }

    double total = k->sewaBulanan + listrik;
    catatTransaksi(nomorKamar, k->namaPenghuni, "BAYAR_SEWA", total, tgl);
    pushLog("BAYAR", nomorKamar,
            k->namaPenghuni + " | " + bulan + " | Rp" + formatRupiah(total));

    cout << "\n  +----------------------------------------------------------+\n";
    cout << "  | PEMBAYARAN BERHASIL DICATAT!                             |\n";
    cout << "  +----------------------------------------------------------+\n";
    cout << "  Penghuni  : " << k->namaPenghuni << "\n";
    cout << "  Kamar     : " << nomorKamar << " (Lantai " << k->lantai << ")\n";
    cout << "  Bulan     : " << bulan << "\n";
    cout << "  Sewa      : Rp" << formatRupiah(k->sewaBulanan) << "\n";
    cout << "  Listrik   : Rp" << formatRupiah(listrik) << "\n";
    cout << "  TOTAL     : Rp" << formatRupiah(total) << "\n";
    cout << "  +----------------------------------------------------------+\n";
}

void laporPerbaikan() {
    cout << "\n+--- LAPORAN PERBAIKAN KAMAR ----------------------------------+\n";
    int kamar = inputInt("  Nomor Kamar yang Bermasalah : ");
    string kel= inputString("  Keterangan Keluhan         : ");
    cout << "  Prioritas: [1] MENDESAK  [2] SEDANG  [3] RINGAN\n";
    int p = inputInt("  Pilih prioritas [1-3]: ");
    string prio = (p==1)?"MENDESAK":(p==2)?"SEDANG":"RINGAN";

    Kamar* k = cariKamar(kamar);
    if (!k) { cout << "  [!] Kamar tidak ditemukan!\n"; return; }

    antrianPerbaikan.push({kamar, kel, prio});
    if (prio == "MENDESAK") {
        k->status = PERBAIKAN;
        string nama = k->namaPenghuni.empty() ? "-" : k->namaPenghuni;
        pushLog("PERBAIKAN", kamar, kel + " [" + prio + "]");
    }
    cout << "  [OK] Laporan perbaikan Kamar " << kamar
         << " [" << prio << "] berhasil dicatat.\n";
}

void cariPenghuniBST() {
    cout << "\n+--- CARI PENGHUNI -------------------------------------------+\n";
    string nama = inputString("  Masukkan nama penghuni: ");
    NodeBST* h = cariBST(rootBST, nama);
    if (h) {
        Kamar* k = cariKamar(h->kamar);
        NodePenghuni* np = cariPenghuni(nama);
        cout << "\n  [OK] Penghuni ditemukan!\n";
        cout << "  Nama      : " << h->nama << "\n";
        cout << "  Kamar     : " << h->kamar;
        if (k) cout << " - " << tipeStr(k->tipe) << " (Lantai " << k->lantai << ")";
        cout << "\n";
        if (np) {
            cout << "  No KTP    : " << np->noKTP << "\n";
            cout << "  No HP     : " << np->noHP  << "\n";
            cout << "  Pekerjaan : " << np->pekerjaan << "\n";
            cout << "  Tgl Masuk : " << np->tanggalMasuk << "\n";
            cout << "  Durasi    : " << np->durasiKontrak << " bulan\n";
        }
        if (k) cout << "  Fasilitas : " << k->fasilitasExtra << "\n";
    } else {
        cout << "  [!] Penghuni '" << nama << "' tidak ditemukan.\n";
    }
}

void navigasiFasilitas() {
    cout << "\n+--- NAVIGASI FASILITAS KOST ----------------------------------+\n";
    tampilkanDenah();
    int dari = inputInt("  Dari node nomor [0-11]: ");
    int ke   = inputInt("  Ke   node nomor [0-11]: ");
    if (dari<0||dari>=MAX_NODES||ke<0||ke>=MAX_NODES) {
        cout << "  [!] Nomor node tidak valid!\n"; return;
    }
    auto jalur = bfsCari(dari, ke);
    cout << "\n  Rute: ";
    if (jalur.empty()) { cout << "Tidak ada jalur!\n"; return; }
    for (int i=0;i<(int)jalur.size();i++) {
        cout << namaNode[jalur[i]];
        if (i+1<(int)jalur.size()) cout << " -> ";
    }
    cout << "\n  (" << jalur.size()-1 << " langkah)\n";
}

void undoAksi() {
    if (undoStack.empty()) { cout << "  [!] Tidak ada aksi yang bisa di-undo.\n"; return; }
    auto a = undoStack.top(); undoStack.pop();
    cout << "  [UNDO] Aksi dibatalkan: [" << a.tipe << "] Kamar "
         << a.kamar << " - " << a.ket << "\n";
}

// ═══════════════════════════════════════════════════════
//  MAIN MENU
// ═══════════════════════════════════════════════════════
void tampilkanMenu() {
    cout << "\n+===============================================================+\n";
    cout << "|         SISTEM MANAJEMEN KOST / KONTRAKAN                    |\n";
    cout << "|   Lantai 1-2: Reguler  |  Lantai 3-4: VIP                   |\n";
    cout << "+===============================================================+\n";
    cout << "| [1]  Daftar Semua Kamar          [6]  Laporan Perbaikan      |\n";
    cout << "| [2]  Kamar Kosong / Tersedia     [7]  Navigasi Fasilitas     |\n";
    cout << "| [3]  Penghuni Masuk (Input)      [8]  Urutkan Kamar (Harga) |\n";
    cout << "| [4]  Penghuni Keluar             [9]  Riwayat Transaksi      |\n";
    cout << "| [5]  Bayar Sewa Bulanan          [10] Penghuni (BST A-Z)     |\n";
    cout << "|                                  [11] Cari Penghuni          |\n";
    cout << "|                                  [12] Antrian Tagihan        |\n";
    cout << "|                                  [13] Antrian Perbaikan      |\n";
    cout << "|                                  [14] Log Aktivitas          |\n";
    cout << "|                                  [15] Undo Aksi Terakhir     |\n";
    cout << "|                                  [0]  Keluar                 |\n";
    cout << "+===============================================================+\n";
    cout << "  Pilih menu: ";
}

int main() {
    inisialisasiKamar();
    bangunGraph();

    cout << "\n  Selamat datang di Sistem Manajemen Kost/Kontrakan!\n";
    cout << "  Total kamar tersedia: " << totalKamar << " kamar\n";
    cout << "  (Lantai 1-2: Regular | Lantai 3-4: VIP)\n";

    int pilihan;
    do {
        tampilkanMenu();
        if (!(cin >> pilihan)) {
            cin.clear(); clearInput(); pilihan = -1;
        } else { clearInput(); }

        switch (pilihan) {
            case 1:  tampilkanSemuaKamar();          break;
            case 2:  tampilkanKamarKosong();          break;
            case 3:  penghunIMasuk();                 break;
            case 4:  penghunIKeluar();                break;
            case 5:  bayarSewa();                     break;
            case 6:  laporPerbaikan();                break;
            case 7:  navigasiFasilitas();             break;
            case 8: {
                cout << "  [1] Termurah ke Termahal  [2] Termahal ke Termurah\n";
                int s = inputInt("  Pilih: ");
                tampilkanSortedHarga(s == 1);
                break;
            }
            case 9:  tampilkanRiwayatTransaksi();     break;
            case 10:
                cout << "\n+--- PENGHUNI AKTIF (BST In-Order A-Z) ---------------+\n";
                if (!rootBST) cout << "  (belum ada penghuni)\n";
                else inOrderBST(rootBST);
                cout << "\n";
                break;
            case 11: cariPenghuniBST();               break;
            case 12: tampilkanAntrianTagihan();        break;
            case 13: tampilkanAntrianPerbaikan();      break;
            case 14: tampilkanLog();                   break;
            case 15: undoAksi();                       break;
            case 0:
                cout << "\n  Terima kasih! Program selesai.\n\n"; break;
            default:
                cout << "  [!] Pilihan tidak valid.\n"; break;
        }
    } while (pilihan != 0);

    for (int i=0;i<totalKamar;i++) delete daftarKamar[i];
    return 0;
}
