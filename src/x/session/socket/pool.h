#ifndef   __NOVEMBERIZING_X_SESSION_SOCKET__POOL__H__
#define   __NOVEMBERIZING_X_SESSION_SOCKET__POOL__H__

#include <x/server/socket.h>
#include <x/session/socket.h>

struct xsessionsocket;
struct xserversocket;
typedef struct xsessionsocket xsessionsocket;
typedef struct xserversocket xserversocket;

struct xsessionsocketpool;
struct xsessionsocketpoolset;

typedef struct xsessionsocketpool xsessionsocketpool;
typedef struct xsessionsocketpoolset xsessionsocketpoolset;

struct xsessionsocketpool
{
    const xsessionsocketpoolset * set;

    xsync * sync;
    xsessionsocket * head;
    xsessionsocket * tail;
    xuint64 size;
    xuint64 capacity;

    xserversocket * serversocket;
};

struct xsessionsocketpoolset
{
    xsessionsocketpool * (*del)(xsessionsocketpool *);
    void (*push)(xsessionsocketpool *, xsessionsocket *);
    xsessionsocket * (*pop)(xsessionsocketpool *);
    void (*rem)(xsessionsocketpool *, xsessionsocket *);
    void (*clear)(xsessionsocketpool *);
    xsessionsocket * (*get)(xsessionsocketpool *);
};

extern xsessionsocketpool * xsessionsocketpoolNew(xuint64 capacity, xserversocket * serversocket);

#define xsessionsocketpoolDel(o)            (o->set->del(o))
#define xsessionsocketpoolPush(o, session)  (o->set->push(o, session))
#define xsessionsocketpoolPop(o)            (o->set->pop(o))
#define xsessionsocketpoolRem(o, session)   (o->set->rem(o, session))
#define xsessionsocketpoolClear(o)          (o->set->clear(o))
#define xsessionsocketpoolGet(o)            (o->set->get(o))

#endif // __NOVEMBERIZING_X_SESSION_SOCKET__POOL__H__
