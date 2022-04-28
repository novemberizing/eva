#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>

#include "socket.h"

static xserversocket * serversocketDel(xserversocket * o);
static xint32 serversocketOpen(xserversocket * o);
static xint64 serversocketRead(xserversocket * o);
static xint64 serversocketWrite(xserversocket * o);
static xint32 serversocketClose(xserversocket * o);
static xint32 serversocketShutdown(xserversocket * o, xint32 how);
static void serversocketPush(xserversocket * o, xsessionsocket * sessionsocket);
static void serversocketRem(xserversocket * o, xsessionsocket * sessionsocket);
static void serversocketClear(xserversocket * o);
static xsessionsocket * serversocketAccept(xserversocket * o);
static void serversocketRel(xserversocket * o, xsessionsocket * sessionsocket);

static xserversocketset virtualSet = {
    serversocketDel,
    serversocketOpen,
    serversocketRead,
    serversocketWrite,
    serversocketClose,
    serversocketShutdown,
    serversocketPush,
    serversocketRem,
    serversocketClear,
    serversocketAccept,
    serversocketRel
};

extern xserversocket * xserversocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen)
{
    xserversocket * o = (xserversocket *) xsocketNew(value, (xsocketset *) xaddressof(virtualSet), sizeof(xserversocket));

    o->domain = domain;
    o->type = type;
    o->protocol = protocol;

    o->address.length = addressLen;
    o->address.value = malloc(o->address.length);

    memcpy(o->address.value, address, o->address.length);

    o->sessionsocketpool = xsessionsocketpoolNew(1024, o);

    return o;
}

static xserversocket * serversocketDel(xserversocket * o)
{
    if(o)
    {
        serversocketShutdown(o, xserversocketshutdown_all);
        serversocketClose(o);

        o->sync = xsyncDel(o->sync);
        o->address.value = xobjectDel(o->address.value);
        o->sessionsocketpool = xsessionsocketpoolDel(o->sessionsocketpool);

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
                xfunctionInfo("xserversocketmode_reuseaddr\n");
                if(setsockopt(o->value, SOL_SOCKET, SO_REUSEADDR, xaddressof(value), sizeof(xint32)) < 0)
                {
                    xfunctionThrow("fail to setsockopt(...) caused by %d\n", errno);
                }
            }
            if(bind(o->value, (struct sockaddr *) o->address.value, (socklen_t) o->address.length) == xsuccess)
            {
                if(listen(o->value, SOMAXCONN) == xsuccess)
                {

                }
                else
                {
                    xfunctionInfo("fail to listen(...) caused by %d\n", errno);
                    serversocketClose(o);
                }
            }
            else
            {
                xfunctionInfo("fail to bind(...) caused by %d\n", errno);
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
    sessionsocket->serversocket.prev = o->tail;
    
    if(sessionsocket->serversocket.prev)
    {
        sessionsocket->serversocket.prev->serversocket.next = sessionsocket;
    }
    else
    {
        o->head = sessionsocket;
    }

    o->tail = sessionsocket;
    o->size = o->size + 1;
    xsyncUnlock(o->sync);
}

static void serversocketRem(xserversocket * o, xsessionsocket * sessionsocket)
{
    if(o)
    {
        xsyncLock(o->sync);
        xsessionsocket * prev = sessionsocket->serversocket.prev;
        xsessionsocket * next = sessionsocket->serversocket.next;
        if(sessionsocket->serversocket.prev)
        {
            prev->serversocket.next = next;
            sessionsocket->serversocket.prev = xnil;
        }
        else
        {
            o->head = sessionsocket;
        }
        if(next)
        {
            next->serversocket.prev = prev;
            sessionsocket->serversocket.next = xnil;
        }
        else
        {
            o->tail = sessionsocket;
        }
        o->size = o->size - 1;
        xsyncUnlock(o->sync);
    }
}
static void serversocketClear(xserversocket * o)
{
    xsyncLock(o->sync);
    xsessionsocket * node = xnil;
    do {
        node = o->head;

        if(node)
        {
            o->head = node->serversocket.next;
            o->head->serversocket.prev = xnil;
            node->serversocket.next = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        o->size = o->size - 1;

        xsessionsocketpoolPush(o->sessionsocketpool, node);
    } while(o->head);
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
            sessionsocket->address.length = addrlen;
            sessionsocket->address.value = malloc(sessionsocket->address.length);
            memcpy(sessionsocket->address.value, xaddressof(addr), sessionsocket->address.length);

            serversocketPush(o, sessionsocket);

            xfunctionInfo("server size => %ld\n", o->size);
            xfunctionInfo("sessionsocketpool size => %ld\n", o->sessionsocketpool->size);
        }
    }
    return sessionsocket;
}

static void serversocketRel(xserversocket * o, xsessionsocket * sessionsocket)
{
    serversocketRem(o, sessionsocket);
    xsessionsocketpoolRel(o->sessionsocketpool, sessionsocket);

    xfunctionInfo("server size => %ld\n", o->size);
    xfunctionInfo("sessionsocketpool size => %ld\n", o->sessionsocketpool->size);
}