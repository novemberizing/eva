#ifndef   __NOVEMBERIZING_X_SESSION__SOCKET__H__
#define   __NOVEMBERIZING_X_SESSION__SOCKET__H__

#include <x/socket.h>
#include <x/server/socket.h>
#include <x/stream.h>

struct xsessionsocketpool;

typedef struct xsessionsocketpool xsessionsocketpool;

#define xsessionsocket_invalid_value    xsocket_invalud_value

#define xsessionsocketshutdown_in       xsocketshutdown_in
#define xsessionsocketshutdown_out      xsocketshutdown_out
#define xsessionsocketshutdown_all      xsocketshutdown_all

#define xsessionsocketmode_nonblock     xsocketmode_nonblock

#define xsessionsocketevent_open        xsocketevent_open
#define xsessionsocketevent_in          xsocketevent_in
#define xsessionsocketevent_out         xsocketevent_out
#define xsessionsocketevent_close       xsocketevent_close
#define xsessionsocketevent_connect     xsocketevent_open
#define xsessionsocketevent_error       xsocketevent_error

struct xsessionsocket;
struct xsessionsocketset;

typedef struct xsessionsocket xsessionsocket;
typedef struct xsessionsocketset xsessionsocketset;

struct xsessionsocket
{
    const xsessionsocketset * set;

    xsync * sync;
    xint32 value;
    xuint32 status;
    xuint32 mode;

    xint32 domain;
    xint32 type;
    xint32 protocol;

    struct {
        xstream * in;
        xstream * out;
    } stream;

    struct {
        xsessionsocketpool * container;
        xsessionsocket * prev;
        xsessionsocket * next;
    } sessionsocketpool;
};

struct xsessionsocketset
{
    xsessionsocket * (*del)(xsessionsocket *);
    xint32 (*open)(xsessionsocket *);
    xint64 (*read)(xsessionsocket *);
    xint64 (*write)(xsessionsocket *);
    xint32 (*close)(xsessionsocket *);
    xint32 (*shutdown)(xsessionsocket *, xint32);
};

extern xsessionsocket * xsessionsocketNew(xint32 value, xsessionsocketpool * sessionsocketpool);

#define xsessionsocketDel(o)                        (o->set->del(o))
#define xsessionsocketOpen(o)                       (o->set->open(o))
#define xsessionsocketRead(o)                       (o->set->read(o))
#define xsessionsocketWrite(o)                      (o->set->write(o))
#define xsessionsocketClose(o)                      (o->set->close(o))
#define xsessionsocketShutdown(o, how)              (o->set->shutdown(o, how))

#endif // __NOVEMBERIZING_X_SESSION__SOCKET__H__
