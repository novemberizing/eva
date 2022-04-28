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
        xserver * container;
        xsession * prev;
        xsession * next;
    } server;

    struct {
        xsessionpool * container;
        xsession * prev;
        xsession * next;
    } sessionpool;

};

struct xsessionset
{
    xsession * (*del)(xsession *);
    xint64 (*read)(xsession *);
    xint64 (*write)(xsession *);
    xint32 (*close)(xsession *);
};

extern xsession * xsessionNew(void);

#define xsessionDel(o)          (o->set->del(o))
#define xsessionRead(o)         (o->set->read(o))
#define xsessionWrite(o)        (o->set->write(o))
#define xsessionClose(o)        (o->set->close(o))

#endif // __NOVEMBERIZING_X__SESSION__H__
