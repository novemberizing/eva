#ifndef   __NOVEMBERIZING_X__SOCKET__H__
#define   __NOVEMBERIZING_X__SOCKET__H__

#include <x/descriptor.h>

#define xsocket_invalud_value       xdescriptor_invalid_value

#define xsocketshutdown_in          1
#define xsocketshutdown_out         2
#define xsocketshutdown_all         3

#define xsocketmode_nonblock        xdescriptormode_nonblock
#define xsocketmode_reuseaddr       (0x00010000 << 0U)

#define xsocketevent_none           xdescriptorevent_none
#define xsocketevent_open           xdescriptorevent_open
#define xsocketevent_in             xdescriptorevent_in
#define xsocketevent_out            xdescriptorevent_out
#define xsocketevent_close          xdescriptorevent_close
#define xsocketevent_error          xdescriptorevent_error
#define xsocketevent_release        xdescriptorevent_release

#define xsocketstatus_none          xdescriptorstatus_none
#define xsocketstatus_open          xdescriptorstatus_open
#define xsocketstatus_in            xdescriptorstatus_in
#define xsocketstatus_out           xdescriptorstatus_out
#define xsocketstatus_close         xdescriptorstatus_close
#define xsocketstatus_error         xdescriptorstatus_error
#define xsocketstatus_release       xdescriptorstatus_release

#define xeventtype_socket_none      xeventtype_descriptor_none
#define xeventtype_socket_open      xeventtype_descriptor_open
#define xeventtype_socket_in        xeventtype_descriptor_in
#define xeventtype_socket_out       xeventtype_descriptor_out
#define xeventtype_socket_close     xeventtype_descriptor_close
#define xeventtype_socket_error     xeventtype_descriptor_error
#define xeventtype_socket_release   xeventtype_descriptor_release

#define xsocketevent_type_none      xdescriptorevent_type_none
#define xsocketevent_type_open      xdescriptorevent_type_open
#define xsocketevent_type_in        xdescriptorevent_type_in
#define xsocketevent_type_out       xdescriptorevent_type_out
#define xsocketevent_type_close     xdescriptorevent_type_close
#define xsocketevent_type_error     xdescriptorevent_type_error
#define xsocketevent_type_release   xdescriptorevent_type_release

struct xsocket;
struct xsocketset;

typedef struct xsocket xsocket;
typedef struct xsocketset xsocketset;

struct xsocketeventsubscription;

typedef struct xsocketeventsubscription xsocketeventsubscription;

struct xsocketset
{
    xsocket * (*del)(xsocket *);

    xint32 (*open)(xsocket *);
    xint64 (*read)(xsocket *);
    xint64 (*write)(xsocket *);
    xint32 (*close)(xsocket *);
    xuint32 (*interest)(xsocket *);

    xint32 (*shutdown)(xsocket *, xint32);
};

struct xsocket
{
    const xsocketset * set;

    xsocketeventsubscription * subscription;
    xsync * sync;

    xint32 value;
    xuint32 status;
    xuint32 mode;
};

extern xsocket * xsocketNew(xint32 value, const xsocketset * set, xuint64 size);

#define xsocketDel(o)               (o->set->del(o))
#define xsocketOpen(o)              (o->set->open(o))
#define xsocketRead(o)              (o->set->read(o))
#define xsocketWrite(o)             (o->set->write(o))
#define xsocketClose(o)             (o->set->close(o))
#define xsocketInterest(o)          (o->set->interest(o))
#define xsocketShutdown(o, how)     (o->set->shutdown(o, how))

#define xsocketSetMode(o, value)    (o->mode = o->mode | value)

#endif // __NOVEMBERIZING_X__SOCKET__H__
