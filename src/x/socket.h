#ifndef   __NOVEMBERIZING_X__SOCKET__H__
#define   __NOVEMBERIZING_X__SOCKET__H__

#include <x/descriptor.h>

#define xsocket_invalud_value       xdescriptor_invalid_value

#define xsocketshutdown_in          1
#define xsocketshutdown_out         2
#define xsocketshutdown_all         3

#define xsocketmode_nonblock        xdescriptormode_nonblock

#define xsocketevent_open           xdescriptorevent_open
#define xsocketevent_in             xdescriptorevent_in
#define xsocketevent_out            xdescriptorevent_out
#define xsocketevent_close          xdescriptorevent_close
#define xsocketevent_error          xdescriptorevent_error

struct xsocket;
struct xsocketset;

typedef struct xsocket xsocket;
typedef struct xsocketset xsocketset;

struct xsocketset
{
    xsocket * (*del)(xsocket *);
    xint32 (*open)(xsocket *);
    xint64 (*read)(xsocket *);
    xint64 (*write)(xsocket *);
    xint32 (*close)(xsocket *);
    xint32 (*shutdown)(xsocket *, xint32);
};

struct xsocket
{
    const xsocketset * set;

    xsync * sync;
    xint32 value;
    xuint32 status;
    xuint32 mode;

    xint32 domain;
    xint32 type;
    xint32 protocol;
};

extern xsocket * xsocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const xsocketset * set, xuint64 size);

#define xsocketDel(o)               (o->set->del(o))
#define xsocketOpen(o)              (o->set->open(o))
#define xsocketRead(o)              (o->set->read(o))
#define xsocketWrite(o)             (o->set->write(o))
#define xsocketClose(o)             (o->set->close(o))
#define xsocketShutdown(o, how)     (o->set->shutdown(o, how))

#define xsocketSetMode(o, value)    (o->mode = o->mode | value)

#endif // __NOVEMBERIZING_X__SOCKET__H__
