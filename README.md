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
### a. Download & Ekstrak (download_and_extract())

❦ Tujuan:

> Unduh Clues.zip dari Google Drive (jika folder Clues/ belum ada).
>
> Ekstrak isinya dan hapus file ZIP.

❦ Algoritma:

> Cek apakah folder Clues/ sudah ada.
>
> Jika belum:
>     Gunakan wget untuk download.
>
> Gunakan unzip untuk ekstrak.
>
> Hapus Clues.zip dengan rm.

Kode:
```
void download_and_extract() {
    struct stat st = {0};
    if (stat(FOLDER_NAME, &st) == 0 && S_ISDIR(st.st_mode)) {
        return; // Folder sudah ada, skip.
    }

    // Download Clues.zip
    char *wget_args[] = {"wget", "-O", "Clues.zip", "https://drive.google.com/...", NULL};
    run_command(wget_args);

    // Ekstrak
    char *unzip_args[] = {"unzip", "Clues.zip", NULL};
    run_command(unzip_args);

    // Hapus ZIP
    char *rm_args[] = {"rm", "Clues.zip", NULL};
    run_command(rm_args);
}
```
Fungsi penduikung (run_command):
```
void run_command(char *argv[]) {
    pid_t pid = fork();
    if (pid == 0) { // Child process
        execvp(argv[0], argv); // Jalankan perintah (e.g., wget, unzip)
        exit(EXIT_FAILURE); // Jika execvp gagal
    } else { // Parent process
        waitpid(pid, NULL, 0); // Tunggu child selesai
    }
}
```

---

### b. Filter File (filter_files())
❦ Tujuan:

> Pindahkan file dengan nama 1 huruf/angka + .txt (e.g., a.txt, 1.txt) ke folder Filtered/.
>
> Hapus file yang tidak valid.

❦ Algoritma:

> Buat folder Filtered/.
>
> Loop melalui ClueA/ sampai ClueD/:
>
> Baca setiap file di direktori.
>
> Jika nama file valid:
>
> Pindahkan ke Filtered/ dengan mv.
>
> Jika tidak valid:
>
> Hapus dengan rm.

#### Kode
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

``` action.c
int is_valid_single_char_filename(const char *filename) {
    // Format: [a-Z0-9].txt (panjang 5: a.txt)
    return (strlen(filename) == 5 && 
            isalnum(filename[0]) && 
            strcmp(filename + 1, ".txt") == 0);
}
```

---

### c. Gabungkan File (combine_files())
❦ Tujuan:

> Gabungkan isi file di Filtered/ ke Combined.txt dengan urutan:
> 
> angka → huruf → angka → huruf (e.g., 1.txt, a.txt, 2.txt, b.txt).
> 
> Hapus file aslinya setelah digabung.

❦ Algoritma:

> Buka folder Filtered/ dan pisahkan file:
>
> digit_files: File dengan nama angka (e.g., 1.txt).
>
> alpha_files: File dengan nama huruf (e.g., a.txt).
> 
> Urutkan kedua kelompok file.
> 
> Gabungkan isinya secara bergantian ke Combined.txt.
> 
> Hapus file sumber.

Kode:
```
void combine_files() {
    DIR *dir = opendir(FILTERED_FOLDER);
    if (!dir) return;

    // Pisahkan file angka dan huruf
    char *digit_files[128], *alpha_files[128];
    int d_count = 0, a_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        if (isdigit(entry->d_name[0])) {
            digit_files[d_count++] = strdup(entry->d_name);
        } else if (isalpha(entry->d_name[0])) {
            alpha_files[a_count++] = strdup(entry->d_name);
        }
    }
    closedir(dir);

    // Urutkan file
    qsort(digit_files, d_count, sizeof(char *), compare_names);
    qsort(alpha_files, a_count, sizeof(char *), compare_names);

    // Gabungkan isi file
    FILE *out = fopen(COMBINED_FILE, "w");
    for (int i = 0; i < (d_count > a_count ? d_count : a_count); i++) {
        if (i < d_count) {
            char file_path[512];
            snprintf(file_path, sizeof(file_path), "%s/%s", FILTERED_FOLDER, digit_files[i]);
            append_file_to_output(file_path, out);
            remove(file_path); // Hapus file sumber
            free(digit_files[i]);
        }
        if (i < a_count) {
            char file_path[512];
            snprintf(file_path, sizeof(file_path), "%s/%s", FILTERED_FOLDER, alpha_files[i]);
            append_file_to_output(file_path, out);
            remove(file_path); // Hapus file sumber
            free(alpha_files[i]);
        }
    }
    fclose(out);
}
```

Fungsi pendukung:

```
// Untuk sorting file
int compare_names(const void *a, const void *b) {
    const char *fa = *(const char **)a;
    const char *fb = *(const char **)b;
    return strcmp(fa, fb); // Urutkan secara leksikografis
}

// Salin isi file ke output
void append_file_to_output(const char *file_path, FILE *out) {
    FILE *in = fopen(file_path, "r");
    if (!in) return;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), in)) {
        fputs(buffer, out);
    }
    fclose(in);
}
```


---

### d. Decode ROT13 (rot13_decode())
❦ Tujuan:

> Decode isi Combined.txt dengan cipher ROT13 (geser 13 huruf).
>
> Hasil disimpan di Decoded.txt.
>
> Algoritma ROT13:
> 
> Untuk setiap karakter:
> 
> Jika huruf (A-Z/a-z):
>
> Geser 13 posisi (e.g., A → N, n → a).
>
> Jika bukan huruf: biarkan as-is.

Kode:
```
void rot13_decode() {
    FILE *in = fopen(COMBINED_FILE, "r");
    FILE *out = fopen(DECODED_FILE, "w");

    int c;
    while ((c = fgetc(in)) != EOF) {
        if (isalpha(c)) {
            if (islower(c)) {
                c = 'a' + (c - 'a' + 13) % 26;
            } else {
                c = 'A' + (c - 'A' + 13) % 26;
            }
        }
        fputc(c, out);
    }

    fclose(in);
    fclose(out);
}
```


---

### Ekstra
_Library yang digunakan:_
```
#include <stdio.h>      // File I/O (fopen, fgets, fprintf, etc.)
#include <stdlib.h>     // Memory allocation (malloc, free), exit()
#include <string.h>     // String operations (strcmp, strdup, strlen)
#include <sys/stat.h>   // File status (stat, mkdir, S_ISDIR)
#include <dirent.h>     // Directory operations (opendir, readdir, closedir)
#include <unistd.h>     // POSIX API (fork, execvp, getpid)
#include <ctype.h>      // Character checks (isalnum, isdigit, isalpha)
#include <sys/types.h>  // Process and file types (pid_t, etc.)
#include <sys/wait.h>   // Process control (wait, waitpid)
```

_Error handling:_

Prints help text if invalid arguments are given.
```
Example Output:
Usage:
  ./action             # Untuk download dan ekstrak Clues.zip
  ./action -m Filter   # Untuk filter file menjadi 1 huruf/angka
  ./action -m Combine  # Untuk menggabungkan isi file
  ./action -m Decode   # Untuk mendecode isi Combined.txt (ROT13)
```


## Soal 2


## Soal 3


## Soal 4
