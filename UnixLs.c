#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h> 

void lsInode(char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char fullpath[1024];

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &statbuf) == -1) {
            perror("lstat");
            exit(1);
        }
        
        printf("%lu %s\n", statbuf.st_ino, entry->d_name);
    }

    closedir(dir);
}


int main(int argc, char* argv[]) {
    char *path = ".";
    int inode_flag = 0;
    int long_flag = 0;

    if (argc > 4 || argc < 2) {
        printf("Usage: %s [-i] [-l] [path]\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[2], "-i") == 0) {
        inode_flag = 1;
    } else if (strcmp(argv[2], "-l") == 0) {
        long_flag = 1;
    } else if (strcmp(argv[2], "-il") == 0 || strcmp(argv[1], "-li") == 0) {
        inode_flag = 1;
        long_flag = 1;
    } else if (argc == 3) {
        if (strcmp(argv[1], "-i") == 0) {
            inode_flag = 1;
        } else if (strcmp(argv[1], "-l") == 0) {
            long_flag = 1;
        } else {
            printf("Usage: %s [-i] [-l] [path]\n", argv[0]);
            return 1;
        }
        path = argv[2];
    } 

    path = argv[argc - 1];
    
    if (inode_flag && long_flag) {
        //lsBothOptions(path);
    } else if (inode_flag) {
        lsInode(path);
    } else if (long_flag) {
        //lsLong(path);
    } else {
        //lsShort(path);
    }

    return 0;
}
