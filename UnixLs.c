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
void printLongDetails(const struct stat *statbuf, const char *name, const char *fullPath);
char *getPermission(mode_t mode);
char *formatDate(time_t time);

int main(int argc, char *argv[])
{
    int inodeFlag = 0, longFlag = 0;
    char *path = ".";   // Default path is current working directory

    // Parse command-line arguments for -i, -l, or both (-il or -li)
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; argv[i][j] != '\0'; j++)
            {
                if (argv[i][j] == 'i')
                {
                    inodeFlag = 1;
                }
                else if (argv[i][j] == 'l')
                {
                    longFlag = 1;
                }
            }
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
            printLongDetails(&buf, dp->d_name, fullPath);
        }
        else if (inodeFlag)
        {
            printInodeDetails(&buf, dp->d_name);
        }
        else if (longFlag)
        {
            printLongDetails(&buf, dp->d_name, fullPath);
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
void printLongDetails(const struct stat *statbuf, const char *name, const char *fullPath)
{
    printf("%s %lu %s %s %5ld %s %s", getPermission(statbuf->st_mode),
           (unsigned long)statbuf->st_nlink, getpwuid(statbuf->st_uid)->pw_name,
           getgrgid(statbuf->st_gid)->gr_name, (long)statbuf->st_size,
           formatDate(statbuf->st_mtime), name);

    if (S_ISLNK(statbuf->st_mode))
    {
        char linkTarget[1024];
        ssize_t len = readlink(fullPath, linkTarget, sizeof(linkTarget) - 1);
        if (len != -1)
        {
            linkTarget[len] = '\0';
            printf(" -> %s", linkTarget);
        }
    }
    printf("\n");
}
char *getPermission(mode_t mode)
{
    static char permissions[11];
    strcpy(permissions, "----------");
    if (S_ISDIR(mode))
        permissions[0] = 'd';
    else if (S_ISLNK(mode))
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
char *formatDate(time_t time)
{
    static char dateStr[20];
    struct tm *timeinfo = localtime(&time);
    strftime(dateStr, sizeof(dateStr), "%b %-d %Y %H:%M", timeinfo);

    return dateStr;
}