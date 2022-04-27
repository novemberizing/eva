#include <poll.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

#include "engine.h"

extern xint64 xeventengineWait(xdescriptor * descriptor, xuint32 events, xint64 second, xint64 nanosecond)
{
    struct pollfd fds = { 0, };

    fds.fd = descriptor->value;
    fds.events = (POLLERR | POLLHUP | POLLNVAL | POLLPRI);
    if(events & xdescriptorevent_in)
    {
        fds.events = fds.events | POLLIN;
    }
    if(events & (xdescriptorevent_out | xdescriptorevent_open))
    {
        fds.events = fds.events | POLLOUT;
    }

    xuint32 collecting = 0;

    struct timespec current = { 0, };
    struct timespec previous = { 0, };
    struct timespec gap = { 0, };

    clock_gettime(CLOCK_REALTIME, xaddressof(previous));

    while(collecting != events)
    {
        int nfds = poll(xaddressof(fds), 1, 1);

        if(nfds > 0)
        {
            if(fds.revents & (POLLERR | POLLHUP | POLLNVAL | POLLPRI))
            {
                collecting = collecting | xdescriptorevent_error;
                break;
            }
            if(fds.revents & POLLOUT)
            {
                if(events & xdescriptorevent_open)
                {
                    xint32 value = 0;
                    socklen_t length = sizeof(xint32);
                    if(getsockopt(fds.fd, SOL_SOCKET, SO_ERROR, xaddressof(value), xaddressof(length)) == xsuccess)
                    {
                        if(value == 0)
                        {
                            collecting = collecting | xdescriptorevent_open;
                        }
                        else if(value != EAGAIN && value != EALREADY && value != EINPROGRESS)
                        {
                            collecting = collecting | xdescriptorevent_error;
                            break;
                        }
                    }
                    else
                    {
                        collecting = collecting | xdescriptorevent_error;
                        break;
                    }
                }
                if(events & xdescriptorevent_out)
                {
                    collecting = collecting | xdescriptorevent_out;
                }
            }
            if(fds.revents & POLLIN)
            {
                collecting = collecting | xdescriptorevent_in;
            }
        }

        if(second > 0 || nanosecond > 0)
        {
            if(collecting != events)
            {
                
                clock_gettime(CLOCK_REALTIME, xaddressof(current));

                gap.tv_sec = current.tv_sec - previous.tv_sec;
                gap.tv_nsec = current.tv_nsec - previous.tv_nsec;
                if(gap.tv_nsec < 0)
                {
                    gap.tv_nsec = gap.tv_nsec + 1000000000;
                    gap.tv_sec = gap.tv_sec - 1;
                }
                gap.tv_sec = gap.tv_sec + gap.tv_nsec / 1000000000;
                gap.tv_nsec = gap.tv_nsec % 1000000000;

                if(second < gap.tv_sec)
                {
                    previous.tv_sec = current.tv_sec;
                    previous.tv_nsec = current.tv_nsec;
                    continue;
                }
                if(nanosecond < gap.tv_nsec)
                {
                    previous.tv_sec = current.tv_sec;
                    previous.tv_nsec = current.tv_nsec;
                    continue;
                }
                break;
            }
        }

    }
    return collecting == events ? xsuccess : xfail;
}
