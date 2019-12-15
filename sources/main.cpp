#include "filesys.h"

int main(int argc, char *argv[]) {

    if (argc < 2)
    {
        path path_to_ftp("misc/ftp");
        iterate(path_to_ftp);
    }
    else {
        path path_to_ftp(argv[1]);
        iterate(path_to_ftp);
    }

    return 0;
}
