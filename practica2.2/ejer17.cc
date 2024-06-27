#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
using namespace std;

int main(int argc, char** argv){
    if (argc != 2){
        cerr << "Debe haber un unico argumento, la ruta al directorio\n";
        return 1;
    }
    struct stat statbuf;
    if (stat(argv[1], &statbuf) == -1){
        perror("stat");
        return 1;
    }
    if ((statbuf.st_mode & S_IFMT) != S_IFDIR){
        cerr << "El fichero a la ruta especificada no es un directorio\n";
        return 1;
    }

    DIR* dd = opendir(argv[1]);
    if (dd == NULL){
        perror ("opendir");
        return 1;
    }
    string dir_name = string(argv[1]);
    struct dirent * entry;
    float kb_size = 0;
    while((entry = readdir(dd)) != NULL){
        struct stat sb_entry;
        const char * entry_path = (dir_name + '/' + string(entry->d_name)).c_str();
        if (lstat(entry_path, &sb_entry) == -1){
            perror("lstat (entry_name)");
            return 1;
        }
        cout << entry->d_name;
        switch (sb_entry.st_mode & S_IFMT) {
            case S_IFREG:
                if (sb_entry.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)){
                    cout << '*';
                }
                kb_size += sb_entry.st_size;
                break;
            case S_IFDIR:
                cout << '/'; 
                break;
            case S_IFLNK:
                ssize_t read_size;
                cout << " -> ";
                char linked_buf[80];
                entry_path = (dir_name + '/' + string(entry->d_name)).c_str();
                read_size = readlink(entry_path, linked_buf, 80);
                if (read_size == -1){
                    perror("readlink");
                    return 1;
                }
                linked_buf[read_size] = '\0';
                cout << linked_buf;
                kb_size += sb_entry.st_size;
                break;
            default:
                break;
        }
        cout << '\n';
    }

    cout << "----------------------------\n";
    cout << "Tamaño en KiB: " << kb_size / 1024 << '\n';

    closedir(dd);

    return 0;
}