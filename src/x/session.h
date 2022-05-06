#ifndef   __NOVEMBERIZING_X__SESSION__H__
#define   __NOVEMBERIZING_X__SESSION__H__

#include <x/server.h>
#include <x/session/pool.h>
#include <x/session/socket.h>

struct xserver;
struct xsessionpool;

typedef struct xserver xserver;
typedef struct xsessionpool xsessionpool;

struct xsession;
struct xsessionset;

typedef struct xsession xsession;
typedef struct xsessionset xsessionset;

struct xsession
{
    const xsessionset * set;

    xsessionsocket * socket;

    struct {
        xserver * server;
        xsessionpool * sessionpool;
        xsession * prev;
        xsession * next;
    } parent;
};

struct xsessionset
{
    xsession * (*del)(xsession *);
    xint32 (*val)(xsession *);
    xint32 (*open)(xsession *);
    xint64 (*read)(xsession *);
    xint64 (*write)(xsession *);
    xint32 (*close)(xsession *);
    xint32 (*interest)(xsession *);
    xint32 (*shutdown)(xsession *, xint32);
};

extern xsession * xsessionNew(void);

#define xsessionDel(o)              (o->set->del(o))
#define xsessionVal(o)              (o->set->val(o))
#define xsessionOpen(o)             (o->set->open(o))
#define xsessionRead(o)             (o->set->read(o))
#define xsessionWrite(o)            (o->set->write(o))
#define xsessionClose(o)            (o->set->close(o))
#define xsessionInterest(o)         (o->set->interest(o))
#define xsessionShutdown(o, how)    (o->set->shutdown(o, how))

#endif // __NOVEMBERIZING_X__SESSION__H__
