#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void listDirectory(const char *path, int inodeFlag, int longFlag);
void printDefault(const char *name);
void printInodeDetails(const struct stat *statbuf, const char *name);
char *getPermission(mode_t mode);

// void printLongDetails(struct stat *statbuf, char *full_path, char *name) {
//     // Print file type and permissions
//     printf((S_ISLNK(statbuf->st_mode)) ? "l" : (S_ISDIR(statbuf->st_mode) ? "d" : "-"));
//     printf((statbuf->st_mode & S_IRUSR) ? "r" : "-");
//     printf((statbuf->st_mode & S_IWUSR) ? "w" : "-");
//     printf((statbuf->st_mode & S_IXUSR) ? "x" : "-");
//     printf((statbuf->st_mode & S_IRGRP) ? "r" : "-");
//     printf((statbuf->st_mode & S_IWGRP) ? "w" : "-");
//     printf((statbuf->st_mode & S_IXGRP) ? "x" : "-");
//     printf((statbuf->st_mode & S_IROTH) ? "r" : "-");
//     printf((statbuf->st_mode & S_IWOTH) ? "w" : "-");
//     printf((statbuf->st_mode & S_IXOTH) ? "x" : "-");

//     // Print number of links
//     printf(" %lu ", statbuf->st_nlink);

//     // Print owner and group
//     struct passwd *pwd = getpwuid(statbuf->st_uid);
//     struct group *grp = getgrgid(statbuf->st_gid);
//     printf("%s %s ", pwd ? pwd->pw_name : "unknown", grp ? grp->gr_name : "unknown");

//     // Print size of the link or file
//     printf("%5ld ", statbuf->st_size);

//     // Print modification date
//     char date[20];
//     strftime(date, sizeof(date), "%b %d %H:%M", localtime(&statbuf->st_mtime));
//     printf("%s ", date);

//     // Print the name of the file or link
//     printf("%s", name);

//     // If it's a symbolic link, print where it points to
//     if (S_ISLNK(statbuf->st_mode)) {
//         char link_target[1024];
//         ssize_t len = readlink(full_path, link_target, sizeof(link_target)-1);
//         if (len != -1) {
//             link_target[len] = '\0'; // Ensure null-termination
//             printf(" -> %s", link_target);
//         }
//     }

//     printf("\n");
// }

// void lsBothOptions(char* path) {
//     DIR *dir;
//     struct dirent *entry;
//     struct stat statbuf;
//     char full_path[1024];

//     // Open the directory
//     dir = opendir(path);
//     if (dir == NULL) {
//         perror("Failed to open directory");
//         exit(EXIT_FAILURE);
//     }

//     // Read and process each directory entry
//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_name[0] == '.') {
//             continue;  // Skip hidden files and directories (., ..)
//         }

//         snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
//         if (lstat(full_path, &statbuf) == -1) {
//             perror("Failed to get file stats");
//             continue;
//         }

//         printf("%lu %s ", statbuf.st_ino, entry->d_name);
//         printLongDetails(&statbuf, full_path, entry->d_name);
//     }

//     // Close the directory
//     closedir(dir);
// }

// void lsInode(char *path) {
//     DIR *dir;
//     struct dirent *entry;
//     struct stat statbuf;
//     char fullpath[1024];

//     dir = opendir(path);
//     if (dir == NULL) {
//         perror("opendir");
//         exit(1);
//     }

//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (entry->d_name[0] == '.') {
//             continue;
//         }

//         snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

//         if (lstat(fullpath, &statbuf) == -1) {
//             perror("lstat");
//             exit(1);
//         }

//         printf("%lu %s\n", statbuf.st_ino, entry->d_name);
//     }

//     closedir(dir);
// }

// void lsShort(char *path) {
//     DIR *dir;
//     struct dirent *entry;

//     dir = opendir(path);
//     if (dir == NULL) {
//         perror("opendir");
//         exit(1);
//     }

//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (entry->d_name[0] == '.') {
//             continue;
//         }

//         printf("%s\n", entry->d_name);
//     }

//     closedir(dir);
// }

int main(int argc, char *argv[])
{
    // char *path = ".";
    // int inode_flag = 0;
    // int long_flag = 0;

    // if (argc > 4 || argc < 2) {
    //     printf("Usage: %s [-i] [-l] [path]\n", argv[0]);
    //     return 1;
    // }

    // if (strcmp(argv[2], "-i") == 0) {
    //     inode_flag = 1;
    // } else if (strcmp(argv[2], "-l") == 0) {
    //     long_flag = 1;
    // } else if (strcmp(argv[2], "-il") == 0 || strcmp(argv[1], "-li") == 0) {
    //     inode_flag = 1;
    //     long_flag = 1;
    // } else if (argc == 3) {
    //     if (strcmp(argv[1], "-i") == 0) {
    //         inode_flag = 1;
    //     } else if (strcmp(argv[1], "-l") == 0) {
    //         long_flag = 1;
    //     } else {
    //         printf("Usage: %s [-i] [-l] [path]\n", argv[0]);
    //         return 1;
    //     }
    //     path = argv[2];
    // }

    // path = argv[argc - 1];

    // if (inode_flag && long_flag) {
    //     lsBothOptions(path);
    // } else if (inode_flag) {
    //     lsInode(path);
    // } else if (long_flag) {
    //     //lsLong(path);
    // } else {
    //     lsShort(path);
    // }

    // return 0;

    int inodeFlag = 0, longFlag = 0;
    char *path = ".";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            inodeFlag = 1;
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            longFlag = 1;
        }
        else
        {
            path = argv[i];
        }
    }

    listDirectory(path, inodeFlag, longFlag);

    return 0;
}

void listDirectory(const char *path, int inodeFlag, int longFlag)
{
    DIR *dir = opendir(path);
    struct dirent *dp;
    struct stat buf;
    char fullPath[1024];

    if (!dir)
    {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, dp->d_name);
        if (lstat(fullPath, &buf) == -1)
        {
            perror("Failed to get file stats for");
            fprintf(stderr, " %s\n", fullPath); // Print the problematic path.
            continue;
        }

        if (inodeFlag && longFlag)
        {
            printf("%lu ", (unsigned long)buf.st_ino);
        }
        else if (inodeFlag)
        {
            printInodeDetails(&buf, dp->d_name);
        }
        else if (longFlag)
        {
            // printLongDetails(&buf, dp->d_name, fullPath);
        }
        else
        {
            printDefault(dp->d_name);
        }
    }

    closedir(dir);
}
void printDefault(const char *name)
{
    printf("%s\n", name);
}
void printInodeDetails(const struct stat *buf, const char *name)
{
    printf("%lu %s\n", (unsigned long)buf->st_ino, name);
}
char *getPermission(mode_t mode)
{
    static char permissions[11];
    strcpy(permissions, "----------");
    if (S_ISDIR(mode))
        permissions[0] = 'd';
    if (S_ISLNK(mode))
        permissions[0] = 'l';

    if (mode & S_IRUSR)
        permissions[1] = 'r';
    if (mode & S_IWUSR)
        permissions[2] = 'w';
    if (mode & S_IXUSR)
        permissions[3] = 'x';
    if (mode & S_IRGRP)
        permissions[4] = 'r';
    if (mode & S_IWGRP)
        permissions[5] = 'w';
    if (mode & S_IXGRP)
        permissions[6] = 'x';
    if (mode & S_IROTH)
        permissions[7] = 'r';
    if (mode & S_IWOTH)
        permissions[8] = 'w';
    if (mode & S_IXOTH)
        permissions[9] = 'x';

    return permissions;
}
