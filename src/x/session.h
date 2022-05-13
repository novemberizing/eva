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

struct xsessioneventsubscription;
typedef struct xsessioneventsubscription xsessioneventsubscription;

struct xsession
{
    const xsessionset * set;

    xsessioneventsubscription * subscription;
    xsync * sync;
    xint32 value;
    xuint32 status;
    xuint32 mode;

    xint32 domain;
    xint32 type;
    xint32 protocol;

    xobject address;

    struct {
        xstream * in;
        xstream * out;
    } stream;

    xsessionpool * sessionpool;
    xserver * server;
    xsession * prev;
    xsession * next;
};

struct xsessionset
{
    xsession * (*del)(xsession *);

    xint32 (*open)(xsession *);
    xint64 (*read)(xsession *);
    xint64 (*write)(xsession *);
    xint32 (*close)(xsession *);
    xint32 (*interest)(xsession *);

    xint32 (*shutdown)(xsession *, xint32);
};

extern xsession * xsessionNew(void);

#define xsessionDel(o)              (o->set->del(o))

#define xsessionOpen(o)             (o->set->open(o))
#define xsessionRead(o)             (o->set->read(o))
#define xsessionWrite(o)            (o->set->write(o))
#define xsessionClose(o)            (o->set->close(o))
#define xsessionInterest(o)         (o->set->interest(o))

#define xsessionShutdown(o, how)    (o->set->shutdown(o, how))

#endif // __NOVEMBERIZING_X__SESSION__H__
