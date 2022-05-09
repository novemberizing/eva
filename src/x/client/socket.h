#ifndef   __NOVEMBERIZING_X_CLIENT__SOCKET__H__
#define   __NOVEMBERIZING_X_CLIENT__SOCKET__H__

#include <x/socket.h>
#include <x/stream.h>
#include <x/object.h>

#define xclientsocket_invalid_value     xsocket_invalud_value

#define xclientsocketshutdown_in        xsocketshutdown_in
#define xclientsocketshutdown_out       xsocketshutdown_out
#define xclientsocketshutdown_all       xsocketshutdown_all

#define xclientsocketmode_nonblock      xsocketmode_nonblock

#define xclientsocketevent_none         xsocketevent_none
#define xclientsocketevent_open         xsocketevent_open
#define xclientsocketevent_in           xsocketevent_in
#define xclientsocketevent_out          xsocketevent_out
#define xclientsocketevent_close        xsocketevent_close
#define xclientsocketevent_connect      xsocketevent_open
#define xclientsocketevent_error        xsocketevent_error

#define xclientsocketstatus_none        xsocketstatus_none
#define xclientsocketstatus_open        xsocketstatus_open
#define xclientsocketstatus_in          xsocketstatus_in
#define xclientsocketstatus_out         xsocketstatus_out
#define xclientsocketstatus_close       xsocketstatus_close
#define xclientsocketstatus_connect     xsocketstatus_open
#define xclientsocketstatus_error       xsocketstatus_error

#define xclientsocketevent_type_none    xsocketevent_type_none
#define xclientsocketevent_type_open    xsocketevent_type_open
#define xclientsocketevent_type_in      xsocketevent_type_in
#define xclientsocketevent_type_out     xsocketevent_type_out
#define xclientsocketevent_type_close   xsocketevent_type_close
#define xclientsocketevent_type_error   xsocketevent_type_error

struct xclientsocket;
struct xclientsocketset;

typedef struct xclientsocket xclientsocket;
typedef struct xclientsocketset xclientsocketset;

struct xclientsocket
{
    const xclientsocketset * set;

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
};

struct xclientsocketset
{
    xclientsocket * (*del)(xclientsocket *);
    xint32 (*val)(xclientsocket *);
    xint32 (*open)(xclientsocket *);
    xint64 (*read)(xclientsocket *);
    xint64 (*write)(xclientsocket *);
    xint32 (*close)(xclientsocket *);
    xuint32 (*interest)(xclientsocket *);
    xint32 (*shutdown)(xclientsocket *, xint32);
    xint32 (*connect)(xclientsocket *);
    xint64 (*send)(xclientsocket *, const unsigned char *, xuint64);
    xint64 (*recv)(xclientsocket *, unsigned char *, xuint64);
};

extern xclientsocket * xclientsocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addresslen);

#define xclientsocketDel(o)                     (o->set->del(o))
#define xclientsocketVal(o)                     (o->set->val(o))
#define xclientsocketOpen(o)                    (o->set->open(o))
#define xclientsocketRead(o)                    (o->set->read(o))
#define xclientsocketWrite(o)                   (o->set->write(o))
#define xclientsocketClose(o)                   (o->set->close(o))
#define xclientsocketInterest(o)                (o->set->interest(o))
#define xclientsocketShutdown(o, how)           (o->set->shutdown(o, how))
#define xclientsocketConnect(o)                 (o->set->connect(o))
#define xclientsocketSend(o, message, length)   (o->set->send(o, message, length))
#define xclientsocketRecv(o, buffer, length)    (o->set->recv(o, buffer, length))

#define xclientsocketSetMode(o, value)          (o->mode = o->mode | value);

#endif // __NOVEMBERIZING_X_CLIENT__SOCKET__H__
