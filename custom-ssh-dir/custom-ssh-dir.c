#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <dlfcn.h>
#include <cstdio>
#include <stdlib.h>

static char init_io = 0;

typedef int (*open_callback_t)(const char *path, int flags, mode_t mode);
static open_callback_t old_open;
static open_callback_t old_open64;


typedef FILE* (*fopen_callback_t)(const char *path, const char *mode);
static fopen_callback_t old_fopen;
static fopen_callback_t old_fopen64;

typedef int (*stat_callback_t)(const char *, struct stat *);
static stat_callback_t old_stat;

void initRealIO()
{
    if (init_io) {
        return;
    }

    old_open = (open_callback_t)dlsym(RTLD_NEXT, "open");
    old_open64 = (open_callback_t)dlsym(RTLD_NEXT, "open64");
    old_fopen = (fopen_callback_t)dlsym(RTLD_NEXT, "fopen");
    old_fopen64 = (fopen_callback_t)dlsym(RTLD_NEXT, "fopen64");
    old_stat = (stat_callback_t)dlsym(RTLD_NEXT, "stat");

    init_io = 1;
}

extern "C"
char *fake_path(const char *p)
{
    char *path = nullptr;
    char *home = getenv("HOME");
    char *ssh_conf_dir = getenv("SSH_CONF_DIR");
    if (home != nullptr && ssh_conf_dir != nullptr)
    {
        const char *SSH = "/.ssh/";
        size_t ssh_len = strlen(home) + strlen(SSH);
        char* SSH_STR =  (char *)calloc(ssh_len + 1, sizeof(char));
        strcpy(SSH_STR, home);
        strcat(SSH_STR, SSH);

        size_t temp_ssh_len = strlen(ssh_conf_dir);

        size_t len = strlen(p) + ssh_len + temp_ssh_len + 1;
        if (strlen(p) >= strlen(SSH_STR) && strncmp(p, SSH_STR, strlen(SSH_STR)) == 0)
        {
            path = (char *)calloc(len, sizeof(char));
            strcpy(path, ssh_conf_dir);
            size_t cplen = strlen(SSH_STR);
            if (ssh_conf_dir[temp_ssh_len - 1] != '/')
            {
                cplen = cplen - 1;
            }
            strcat(path, p + cplen);
        }
        free(SSH_STR);
    }
    if (path == nullptr)
    {
        path = (char *)calloc(strlen(p) + 1, sizeof(char));
        strcpy(path, p);
    }
    return path;
}

extern "C"
int custom_open(const char  *p, int flags, mode_t mode, open_callback_t f)
{
    int fd = -1;
    char *path = fake_path(p);

    fd = f(path, flags, mode);

    free(path);
    return fd;
}

extern "C"
int open (const char  *p, int flags, mode_t mode)
{
    initRealIO();
    return custom_open(p, flags, mode, old_open);
}

extern "C"
int open64 (const char  *p, int flags, mode_t mode)
{
    initRealIO();
    return custom_open(p, flags, mode, old_open64);
}

extern "C"
FILE *custom_fopen(const char *p, const char *mode, fopen_callback_t f)
{
    FILE *file = nullptr;
    char *path = fake_path(p);

    file = f(path, mode);

    free(path);
    return file;
}

extern "C"
FILE *fopen(const char *path, const char *mode)
{
    initRealIO();
    return custom_fopen(path, mode, old_fopen);
}
extern "C"
FILE *fopen64(const char *path, const char *mode)
{
    initRealIO();
    return custom_fopen(path, mode, old_fopen64);
}


extern "C"
int stat(const char * p, struct stat * statbuf)
{
    int status = -1;
    char *path = fake_path(p);

    status = old_stat(path, statbuf);

    free(path);
    return status;
}

