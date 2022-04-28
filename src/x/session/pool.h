#ifndef   __NOVEMBERIZING_X_SESSION__POOL__H__
#define   __NOVEMBERIZING_X_SESSION__POOL__H__

#include <x/session.h>

struct xsession;
typedef struct xsession xsession;


struct xsessionpool;
struct xsessionpoolset;

typedef struct xsessionpool xsessionpool;
typedef struct xsessionpoolset xsessionpoolset;

struct xsessionpool
{
    const xsessionpoolset * set;

    xsync * sync;

    xsession * head;
    xsession * tail;
    xuint64 size;
    xuint64 capacity;
};

struct xsessionpoolset
{
    xsessionpool * (*del)(xsessionpool *);
    xsession * (*get)(xsessionpool *, xsessionsocket *);
    void (*release)(xsessionpool *, xsession *);
};

extern xsessionpool * xsessionpoolNew(xuint64 capacity);

#define xsessionpoolDel(o)                      (o->set->del(o))
#define xsessionpoolGet(o, sessionsocket)       (o->set->get(o, sessionsocket))
#define xsessionpoolRel(o, session)             (o->set->release(o, session))

#endif // __NOVEMBERIZING_X_SESSION__POOL__H__
