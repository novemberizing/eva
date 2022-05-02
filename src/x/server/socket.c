#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

#include "socket.h"

static xserversocket * serversocketDel(xserversocket * o);
static xint32 serversocketOpen(xserversocket * o);
static xint64 serversocketRead(xserversocket * o);
static xint64 serversocketWrite(xserversocket * o);
static xint32 serversocketClose(xserversocket * o);
static xint32 serversocketShutdown(xserversocket * o, xint32 how);
static xsessionsocket * serversocketAccept(xserversocket * o);
static void serversocketRel(xserversocket * o, xsessionsocket * sessionsocket);

static xserversocketset virtualSet = {
    serversocketDel,

    serversocketOpen,
    serversocketRead,
    serversocketWrite,
    serversocketClose,
    serversocketShutdown,

    serversocketAccept,
    serversocketRel
};

static void serversocketPush(xserversocket * o, xsessionsocket * sessionsocket);
static void serversocketRem(xserversocket * o, xsessionsocket * sessionsocket);
static void serversocketClear(xserversocket * o);

extern xserversocket * xserversocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addresslen)
{
    xserversocket * o = (xserversocket *) xsocketNew(value, (xsocketset *) xaddressof(virtualSet), sizeof(xserversocket));

    o->domain = domain;
    o->type = type;
    o->protocol = protocol;

    xobjectSet(xaddressof(o->address), address, addresslen);

    o->sessionsocketpool = xsessionsocketpoolNew(1024, o);

    return o;
}

static xserversocket * serversocketDel(xserversocket * o)
{
    if(o)
    {
        serversocketShutdown(o, xserversocketshutdown_all);
        serversocketClose(o);

        serversocketClear(o);
        o->sessionsocketpool = xsessionsocketpoolDel(o->sessionsocketpool);

        o->sync = xsyncDel(o->sync);
        xobjectSet(xaddressof(o->address), xnil, 0);

        free(o);
    }
    return o;
}

static xint32 serversocketOpen(xserversocket * o)
{
    if(o->value < 0)
    {
        o->value = socket(o->domain, o->type, o->protocol);
        if(o->value >= 0)
        {
            if(o->mode & xserversocketmode_reuseaddr)
            {
                xint32 value = 1;
                xfunctionInfo("xserversocketmode_reuseaddr");
                if(setsockopt(o->value, SOL_SOCKET, SO_REUSEADDR, xaddressof(value), sizeof(xint32)) < 0)
                {
                    xfunctionThrow("fail to setsockopt(...) caused by %d", errno);
                }
            }
            if(o->mode & xserversocketmode_nonblock)
            {
                xint32 flags = fcntl(o->value, F_GETFL, 0);
                fcntl(o->value, F_SETFL, flags | O_NONBLOCK);
            }
            if(bind(o->value, (struct sockaddr *) o->address.value, (socklen_t) o->address.length) == xsuccess)
            {
                if(listen(o->value, SOMAXCONN) == xsuccess)
                {

                }
                else
                {
                    xfunctionInfo("fail to listen(...) caused by %d", errno);
                    serversocketClose(o);
                }
            }
            else
            {
                xfunctionInfo("fail to bind(...) caused by %d", errno);
                serversocketClose(o);
            }
        }
        else
        {
            xfunctionInfo("fail to socket(...) caused by %d\n", errno);
            // 소켓을 생성하지 못한다.
        }
    }
    return o->value > 0 ? xsuccess : xfail;
}

static xint64 serversocketRead(xserversocket * o)
{
}

static xint64 serversocketWrite(xserversocket * o)
{
    return o->value >= 0 ? xsuccess : xfail;
}

static xint32 serversocketClose(xserversocket * o)
{
    if(o->value >= 0)
    {
        xint32 ret = close(o->value);
        o->value = xserversocket_invalid_value;
    }
    return xsuccess;
}

static xint32 serversocketShutdown(xserversocket * o, xint32 how)
{
    if(o->value >= 0)
    {
        if(how == xserversocketshutdown_all)
        {
            how = SHUT_RDWR;
        }
        else if(how == xserversocketshutdown_out)
        {
            how = SHUT_WR;
        }
        else if(how == xserversocketshutdown_in)
        {
            how = SHUT_RD;
        }
        xint32 ret = shutdown(o->value, how);
    }
    return xsuccess;
}

static void serversocketPush(xserversocket * o, xsessionsocket * sessionsocket)
{
    xsyncLock(o->sync);

    sessionsocket->parent.prev = o->tail;
    
    if(sessionsocket->parent.prev)
    {
        sessionsocket->parent.prev->parent.next = sessionsocket;
    }
    else
    {
        o->head = sessionsocket;
    }

    sessionsocket->parent.serversocket = o;

    o->tail = sessionsocket;
    o->size = o->size + 1;
    xsyncUnlock(o->sync);
}

static void serversocketRem(xserversocket * o, xsessionsocket * sessionsocket)
{
    if(o)
    {
        xsyncLock(o->sync);
        xsessionsocket * prev = sessionsocket->parent.prev;
        xsessionsocket * next = sessionsocket->parent.next;
        if(sessionsocket->parent.prev)
        {
            prev->parent.next = next;
            sessionsocket->parent.prev = xnil;
        }
        else
        {
            o->head = next;
        }
        if(next)
        {
            next->parent.prev = prev;
            sessionsocket->parent.next = xnil;
        }
        else
        {
            o->tail = prev;
        }
        sessionsocket->parent.serversocket = xnil;
        o->size = o->size - 1;
        xsyncUnlock(o->sync);
    }
}
static void serversocketClear(xserversocket * o)
{
    xsyncLock(o->sync);
    xsessionsocket * node = xnil;
    while(o->head)
    {
        node = o->head;

        o->head = node->parent.next;

        if(o->head)
        {
            node->parent.next = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        o->size = o->size - 1;
        node->parent.serversocket = xnil;

        xsessionsocketpoolPush(o->sessionsocketpool, node);
    }
    xsyncUnlock(o->sync);
}

static xsessionsocket * serversocketAccept(xserversocket * o)
{
    xsessionsocket * sessionsocket = xnil;
    if(o->value >= 0)
    {
        struct sockaddr addr;
        socklen_t addrlen = sizeof(struct sockaddr);

        int value = accept(o->value, xaddressof(addr), xaddressof(addrlen));

        if(value >= 0)
        {
            sessionsocket = xsessionsocketpoolGet(o->sessionsocketpool);
            sessionsocket->value = value;

            xobjectSet(xaddressof(sessionsocket->address), xaddressof(addr), addrlen);

            serversocketPush(o, sessionsocket);
        }
    }
    return sessionsocket;
}

static void serversocketRel(xserversocket * o, xsessionsocket * sessionsocket)
{
    serversocketRem(o, sessionsocket);

    xsessionsocketpoolRel(o->sessionsocketpool, sessionsocket);
}
