#ifndef   __NOVEMBERIZING_X_SERVER__SOCKET__H__
#define   __NOVEMBERIZING_X_SERVER__SOCKET__H__

#include <x/socket.h>

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
};

struct xserversocketset
{
    xserversocket * (*del)(xserversocket *);
    xint32 (*open)(xserversocket *);
    xint64 (*read)(xserversocket *);
    xint64 (*write)(xserversocket *);
    xint32 (*close)(xserversocket *);
    xint32 (*shutdown)(xserversocket *, xint32);
};

extern xserversocket * xserversocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen);

#define xserversocketDel(o)                 (o->set->del(o))
#define xserversocketOpen(o)                (o->set->open(o))
#define xserversocketRead(o)                (o->set->read(o))
#define xserversocketWrite(o)               (o->set->write(o))
#define xserversocketClose(o)               (o->set->close(o))
#define xserversocketShutdown(o, how)       (o->set->shutdown(o, how))

#endif // __NOVEMBERIZING_X_SERVER__SOCKET__H__
