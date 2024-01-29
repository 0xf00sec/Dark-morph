#include "includes.h"

typedef 
struct _SELFDEL
{
    unsigned char opCodes[CODESIZE];
    int (*fnUnlink)(const char *); 
    void (*fnExit)
    (int);           
    char FileName[PATH_MAX];     
} SELFDEL;

static void remote_thread(SELFDEL *remote)
{
    pthread_join(pthread_self(), NULL);

    while (remote->fnUnlink(remote->FileName) != 0)
    {
        sleep(1);
    }

    remote->fnExit(0);
}

int SelfDelete(const char *executableName)
{
    SELFDEL local = {0};
    pid_t pid = fork();

    if (pid == 0) {
        
        memcpy(local.opCodes, &remote_thread, CODESIZE); 

        
        local.fnUnlink = unlink; 
        local.fnExit = exit;     

        
        getcwd(local.FileName, PATH_MAX);
        strcat(local.FileName, "/");
        strcat(local.FileName, executableName);

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_t tid;

        pthread_create(&tid, &attr, (void *(*)(void *))&remote_thread, &local);
        pthread_attr_destroy(&attr);

        sleep(1);
    }
    return 0;
}