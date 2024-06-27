#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
using namespace std;




int main(int argc, char** argv){
   if(argc < 2){
       cout << "Hace falta un argumento con el nombre del fichero\n";
       return 1;
   }
   struct stat sb;
   if (lstat(argv[1], &sb) == -1) {
       perror("lstat");
       return 1;
   }


   if ((sb.st_mode & S_IFMT) != S_IFREG){
       cout << "El fichero no es regular, no hago nada\n";
       return 0;
   }
   string file_path = string(argv[1]);
   if (link(argv[1], (file_path + ".hard").c_str()) == -1){
       perror("link");
       return 1;
   }
   if (symlink(argv[1], (file_path + ".sym").c_str()) == -1){
       perror("symlink");
       return 1;
   }
   cout << "Creado el enlace rigido y simbolico\n";


   return 0;
}
