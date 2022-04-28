#ifndef   __NOVEMBERIZING_X_SERVER__SOCKET__H__
#define   __NOVEMBERIZING_X_SERVER__SOCKET__H__

#include <x/socket.h>
#include <x/session/socket/pool.h>

struct xsessionsocket;
struct xsessionsocketpool;

typedef struct xsessionsocket xsessionsocket;
typedef struct xsessionsocketpool xsessionsocketpool;

#define xserversocket_invalid_value     xsocket_invalud_value

#define xserversocketshutdown_in        xsocketshutdown_in
#define xserversocketshutdown_out       xsocketshutdown_out
#define xserversocketshutdown_all       xsocketshutdown_all

struct xserversocket;
struct xserversocketset;

typedef struct xserversocket xserversocket;
typedef struct xserversocketset xserversocketset;

struct xserversocket
{
    const xserversocketset * set;

    xsync * sync;
    xint32 value;
    xuint32 status;
    xuint32 mode;

    xint32 domain;
    xint32 type;
    xint32 protocol;

    struct {
        xuint64 length;
        void * value;
    } address;

    xsessionsocketpool * sessionsocketpool;

    xsessionsocket * head;
    xsessionsocket * tail;
    xuint64 size;
};

struct xserversocketset
{
    xserversocket * (*del)(xserversocket *);
    xint32 (*open)(xserversocket *);
    xint64 (*read)(xserversocket *);
    xint64 (*write)(xserversocket *);
    xint32 (*close)(xserversocket *);
    xint32 (*shutdown)(xserversocket *, xint32);
    void (*push)(xserversocket *, xsessionsocket *);
    void (*rem)(xserversocket *, xsessionsocket *);
    void (*clear)(xserversocket *);
    xsessionsocket * (*accept)(xserversocket *);
};

extern xserversocket * xserversocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen);

#define xserversocketDel(o)                 (o->set->del(o))
#define xserversocketOpen(o)                (o->set->open(o))
#define xserversocketRead(o)                (o->set->read(o))
#define xserversocketWrite(o)               (o->set->write(o))
#define xserversocketClose(o)               (o->set->close(o))
#define xserversocketShutdown(o, how)       (o->set->shutdown(o, how))
#define xserversocketPush(o, session)       (o->set->push(o, session))
#define xserversocketRem(o, session)        (o->set->rem(o, session))
#define xserversocketClear(o, session)      (o->set->clear(o))
#define xserversocketAccept(o)              (o->set->accept(o))

#endif // __NOVEMBERIZING_X_SERVER__SOCKET__H__
