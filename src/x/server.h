#ifndef   __NOVEMBERIZING_X__SERVER__H__
#define   __NOVEMBERIZING_X__SERVER__H__

#include <x/server/socket.h>
#include <x/session/pool.h>

struct xsession;
struct xsessionpool;
typedef struct xsession xsession;
typedef struct xsessionpool xsessionpool;

struct xserver;
struct xserverset;

typedef struct xserver xserver;
typedef struct xserverset xserverset;

struct xserver
{
    const xserverset * set;

    xserversocket * socket;
    xsessionpool * sessionpool;

    xsync * sync;

    xsession * head;
    xsession * tail;
    xuint64 size;
};

struct xserverset
{
    xserver * (*del)(xserver *);
    xint32 (*open)(xserver *);
    xint32 (*close)(xserver *);
    xsession * (*accept)(xserver *);
    void (*release)(xserver *, xsession *);
};

extern xserver * xserverNew(xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen, xsessionpool * sessionpool);

#define xserverDel(o)           (o->set->del(o))
#define xserverOpen(o)          (o->set->open(o))
#define xserverClose(o)         (o->set->close(o))
#define xserverAccept(o)        (o->set->accept(o))
#define xserverRel(o, session)  (o->set->release(o, session))

#endif // __NOVEMBERIZING_X__SERVER__H__
