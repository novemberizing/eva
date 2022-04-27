#include <stdio.h>

#include <x/sync.h>

int main(int argc, char ** argv)
{
    xsync * sync = xsyncNew(xsynctype_mutex);

    printf("%p\n", sync);

    xsyncLock(sync);
    xsyncWait(sync, 1, 0);
    xsyncUnlock(sync);

    sync = xsyncDel(sync);
    printf("%p\n", sync);
    return 0;
}