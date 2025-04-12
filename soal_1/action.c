#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>

#define FOLDER_NAME "Clues"
#define FILTERED_FOLDER "Filtered"
#define COMBINED_FILE "Combined.txt"
#define DECODED_FILE "Decoded.txt"


int is_valid_single_char_filename(const char *filename) {
    if (strlen(filename) != 5) return 0;
    if (!isalnum(filename[0])) return 0;
    return strcmp(filename + 1, ".txt") == 0;
}

int is_digit_filename(const char *filename) {
    return isdigit(filename[0]);
}

int compare_names(const void *a, const void *b) {
    const char *fa = *(const char **)a;
    const char *fb = *(const char **)b;
    int ida = isdigit(fa[0]);
    int idb = isdigit(fb[0]);
    if (ida != idb) return ida ? -1 : 1;
    return strcmp(fa, fb);
}

void download_and_extract() {
    struct stat st = {0};
    if (stat(FOLDER_NAME, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("Folder %s sudah ada. Skip download.\n", FOLDER_NAME);
        return;
    }

    printf("Mengunduh Clues.zip...\n");
    system("wget -O Clues.zip \"https://drive.google.com/uc?export=download&id=1xFn1OBJUuSdnApDseEczKhtNzyGekauK\"");


    printf("Ekstrak Clues.zip...\n");
    system("unzip Clues.zip > /dev/null");

    printf("Menghapus Clues.zip...\n");
    system("rm Clues.zip");
}

void filter_files() {
    mkdir(FILTERED_FOLDER, 0777);

    struct dirent *entry;
    char path[512], full_path[512], command[512];

    for (char folder = 'A'; folder <= 'D'; folder++) {
        snprintf(path, sizeof(path), "%s/Clue%c", FOLDER_NAME, folder);
        DIR *dir = opendir(path);
        if (!dir) continue;

        while ((entry = readdir(dir)) != NULL) {
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

            struct stat st;
            if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode)) {
                if (is_valid_single_char_filename(entry->d_name)) {
                    snprintf(command, sizeof(command), "mv \"%s\" %s/", full_path, FILTERED_FOLDER);
                    system(command);
                } else {
                    snprintf(command, sizeof(command), "rm \"%s\"", full_path);
                    system(command);
                }
            }
        }
        closedir(dir);
    }

    printf("Filtering selesai.\n");
}

void combine_files() {
    DIR *dir = opendir(FILTERED_FOLDER);
    if (!dir) {
        perror("Gagal membuka folder Filtered");
        return;
    }

    char *digit_files[128];
    char *alpha_files[128];
    int d_count = 0, a_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        if (isdigit(entry->d_name[0]))
            digit_files[d_count++] = strdup(entry->d_name);
        else if (isalpha(entry->d_name[0]))
            alpha_files[a_count++] = strdup(entry->d_name);
    }
    closedir(dir);

    qsort(digit_files, d_count, sizeof(char *), compare_names);
    qsort(alpha_files, a_count, sizeof(char *), compare_names);

    FILE *out = fopen(COMBINED_FILE, "w");
    if (!out) {
        perror("Gagal membuka Combined.txt");
        return;
    }

    int max = d_count > a_count ? d_count : a_count;
    char path[512], buffer[1024];

    for (int i = 0; i < max; i++) {
        if (i < d_count) {
            snprintf(path, sizeof(path), "%s/%s", FILTERED_FOLDER, digit_files[i]);
            FILE *in = fopen(path, "r");
            if (in) {
                while (fgets(buffer, sizeof(buffer), in)) fputs(buffer, out);
                fclose(in);
                remove(path);
            }
            free(digit_files[i]);
        }

        if (i < a_count) {
            snprintf(path, sizeof(path), "%s/%s", FILTERED_FOLDER, alpha_files[i]);
            FILE *in = fopen(path, "r");
            if (in) {
                while (fgets(buffer, sizeof(buffer), in)) fputs(buffer, out);
                fclose(in);
                remove(path);
            }
            free(alpha_files[i]);
        }
    }

    fclose(out);
    printf("Penggabungan selesai ke %s.\n", COMBINED_FILE);
}


void rot13_decode() {
    FILE *in = fopen(COMBINED_FILE, "r");
    if (!in) {
        perror("Combined.txt tidak ditemukan");
        return;
    }

    FILE *out = fopen(DECODED_FILE, "w");
    if (!out) {
        perror("Gagal membuat Decoded.txt");
        fclose(in);
        return;
    }

    int ch;
    while ((ch = fgetc(in)) != EOF) {
        if ('a' <= ch && ch <= 'z') ch = ((ch - 'a' + 13) % 26) + 'a';
        else if ('A' <= ch && ch <= 'Z') ch = ((ch - 'A' + 13) % 26) + 'A';
        fputc(ch, out);
    }

    fclose(in);
    fclose(out);
    printf("Decoded.txt berhasil dibuat.\n");
}

void print_usage() {
    printf("Usage:\n");
    printf("  ./action             # Untuk download dan ekstrak Clues.zip\n");
    printf("  ./action -m Filter   # Untuk filter file menjadi 1 huruf/angka\n");
    printf("  ./action -m Combine  # Untuk menggabungkan isi file\n");
    printf("  ./action -m Decode   # Untuk mendecode isi Combined.txt (ROT13)\n");
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        download_and_extract();
    } else if (argc == 3 && strcmp(argv[1], "-m") == 0) {
        if (strcmp(argv[2], "Filter") == 0) {
            filter_files();
        } else if (strcmp(argv[2], "Combine") == 0) {
            combine_files();
        } else if (strcmp(argv[2], "Decode") == 0) {
            rot13_decode();
        } else {
            print_usage();
        }
    } else {
        print_usage();
    }
    return 0;
}
