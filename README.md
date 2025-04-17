# Laporan Penjelasan dan Penyelesaian Soal Modul 2

بِسْمِ اللهِ الرَّحْمٰنِ الرَّحِيْمِ

Segala puji bagi Allah, Rabb semesta alam. Shalawat dan salam semoga tercurah kepada Nabi Muhammad ﷺ, keluarga, sahabat, serta siapa saja yang mengikuti jalan beliau hingga hari kiamat.

Laporan ini disusun sebagai bentuk tanggung jawab akademik dan ikhtiar dalam menuntut ilmu, sebagaimana sabda Nabi ﷺ:

"Barangsiapa menempuh jalan untuk menuntut ilmu, niscaya Allah akan mudahkan baginya jalan menuju surga."

(HR. Muslim)

Semoga apa yang tertulis dalam laporan ini menjadi sebab keberkahan, ilmu yang bermanfaat, dan amal yang diterima di sisi Allah ﷻ.

Semoga Allah memudahkan langkah kita semua dalam menuntut ilmu, mengamalkannya, serta menjaganya agar tidak sekadar jadi hafalan di otak, tapi bekal untuk akhirat.


## Anggota Kelompok
| Nama                      | NRP        |
|---------------------------|------------|
|Muhammad Hikari Reiziq R.  | 5027241079 |
|Dira Muhammad Ilyas S. A.  | 5027241033 |
|Thariq Kyran Aryunaldi     | 5027241073 |

## Soal 1
### 1. Download & Ekstrak (download_and_extract())

Tujuan:

Unduh Clues.zip dari Google Drive (jika folder Clues/ belum ada).

Ekstrak isinya dan hapus file ZIP.

Algoritma:

Cek apakah folder Clues/ sudah ada.

Jika belum:

Gunakan wget untuk download.

Gunakan unzip untuk ekstrak.

Hapus Clues.zip dengan rm.


---

## 2. Filter File (filter_files())
Tujuan:
Pindahkan file dengan nama 1 huruf/angka + .txt (e.g., a.txt, 1.txt) ke folder Filtered/.
Hapus file yang tidak valid.
Algoritma:
Buat folder Filtered/.
Loop melalui ClueA/ sampai ClueD/:
Baca setiap file di direktori.
Jika nama file valid:
-> Pindahkan ke Filtered/ dengan mv.
Jika tidak valid:
-> Hapus dengan rm.

``` action.c

void filter_files() {
    mkdir(FILTERED_FOLDER, 0777); // Buat folder Filtered
    for (char folder = 'A'; folder <= 'D'; folder++) {
        char path[512];
        snprintf(path, sizeof(path), "%s/Clue%c", FOLDER_NAME, folder);

        DIR *dir = opendir(path);
        if (!dir) continue; // Skip jika folder tidak ada

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // Abaikan file hidden (., ..)
            if (entry->d_name[0] == '.') continue;

            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

            // Cek apakah nama file valid (e.g., a.txt, 1.txt)
            if (is_valid_single_char_filename(entry->d_name)) {
                char *mv_args[] = {"mv", full_path, FILTERED_FOLDER, NULL};
                run_command(mv_args);
            } else {
                char *rm_args[] = {"rm", full_path, NULL};
                run_command(rm_args);
            }
        }
        closedir(dir);
    }
}
```

Fungsi untuk memvalidasi nama file:

'''action.c
int is_valid_single_char_filename(const char *filename) {
    // Format: [a-Z0-9].txt (panjang 5: a.txt)
    return (strlen(filename) == 5 && 
            isalnum(filename[0]) && 
            strcmp(filename + 1, ".txt") == 0);
}
'''

---

### 3. Gabungkan File (combine_files())
Tujuan:

Gabungkan isi file di Filtered/ ke Combined.txt dengan urutan:
angka → huruf → angka → huruf (e.g., 1.txt, a.txt, 2.txt, b.txt).

Hapus file aslinya setelah digabung.

Algoritma:

Buka folder Filtered/ dan pisahkan file:

digit_files: File dengan nama angka (e.g., 1.txt).

alpha_files: File dengan nama huruf (e.g., a.txt).

Urutkan kedua kelompok file.

Gabungkan isinya secara bergantian ke Combined.txt.

Hapus file sumber.

---

### 4. Decode ROT13 (rot13_decode())
Tujuan:

Decode isi Combined.txt dengan cipher ROT13 (geser 13 huruf).

Hasil disimpan di Decoded.txt.

Algoritma ROT13:

Untuk setiap karakter:

Jika huruf (A-Z/a-z):

Geser 13 posisi (e.g., A → N, n → a).

Jika bukan huruf: biarkan as-is.

---

### Ekstra
> Library yang digunakan:

> Error handling:


## Soal 2


## Soal 3


## Soal 4
