# Sistem Manajemen Kost/Kontrakan

Aplikasi sistem manajemen kost/kontrakan berbasis console (CLI) yang dikembangkan menggunakan bahasa pemrograman C++ dengan penerapan berbagai Struktur Data. Proyek ini dibuat sebagai tugas UAS Mata Kuliah Struktur Data.

## Deskripsi

Program ini memungkinkan pemilik kost untuk mengelola data kamar, mencatat penghuni masuk dan keluar, memproses pembayaran sewa bulanan, melaporkan kerusakan/perbaikan kamar, mencari data penghuni dengan cepat, mengurutkan kamar berdasarkan harga sewa, hingga menampilkan rute navigasi antar fasilitas kost. Kost terdiri dari 4 lantai dengan total 40 kamar, dimana Lantai 1–2 merupakan kamar Reguler dan Lantai 3–4 merupakan kamar VIP.

## Struktur Data yang Diterapkan

- **Array + Pointer** – Daftar 40 kamar (`Kamar* daftarKamar[MAX_KAMAR]`) sebagai data master kost
- **Linked List** – Menyimpan data penghuni aktif (`NodePenghuni`) dan riwayat transaksi (`NodeTransaksi`) secara dinamis
- **Stack** – Mencatat log aktivitas dan mendukung fitur undo aksi terakhir
- **Queue** – Antrian tagihan sewa dan antrian laporan perbaikan kamar
- **Binary Search Tree (BST)** – Pencarian data penghuni berdasarkan nama secara efisien (O(log n))
- **Graph + BFS** – Pemetaan denah kost (pintu masuk, parkir, lantai, dapur, mushola, dsb) dan pencarian rute terpendek antar fasilitas
- **Sorting (Bubble Sort)** – Mengurutkan daftar kamar berdasarkan harga sewa, termurah ke termahal atau sebaliknya
- **Searching (Linear & Sequential Search)** – Pencarian kamar berdasarkan nomor dan kamar kosong berdasarkan tipe

## Struktur Project

```
SistemManajemenKost/
└── kost_management.cpp
```

> Catatan: seluruh logika program (struct, fungsi struktur data, dan menu) digabung dalam satu file `kost_management.cpp` untuk mempermudah kompilasi dan pengumpulan tugas.

## Cara Menjalankan

### Persyaratan

- Pastikan di laptop sudah terinstall compiler C++ (GCC/G++ atau MinGW) yang mendukung standar C++11 ke atas

### Kompilasi dan Eksekusi

```bash
# Compile program
g++ -o kost_management kost_management.cpp

# Jalankan program
./kost_management
```

Khusus pengguna Windows (MinGW):

```bash
g++ -o kost_management.exe kost_management.cpp
kost_management.exe
```

## Fitur Utama

1. Daftar semua kamar (per lantai, tipe, status, dan penghuni)
2. Lihat kamar kosong / tersedia
3. Input penghuni masuk
4. Proses penghuni keluar (termasuk pengembalian deposit)
5. Pembayaran sewa bulanan (sewa + tagihan listrik)
6. Laporan perbaikan kamar dengan prioritas (Mendesak/Sedang/Ringan)
7. Navigasi rute antar fasilitas kost (BFS)
8. Urutkan kamar berdasarkan harga sewa (termurah/termahal)
9. Riwayat transaksi (pembayaran, masuk, keluar)
10. Daftar penghuni aktif terurut A–Z (BST in-order)
11. Cari penghuni berdasarkan nama (BST)
12. Lihat antrian tagihan sewa
13. Lihat antrian laporan perbaikan
14. Lihat log aktivitas
15. Undo aksi terakhir

## Anggota Kelompok 6

- Diki Aryadi
- Hilal Shofar Falih
- Muhamad Dani
- Sesil Praya

## Mata Kuliah

Struktur Data – Semester Genap 2025/2026
Dosen Pengampu: Popon Dauni, S.T., M.Kom.
