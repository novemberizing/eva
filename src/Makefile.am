AM_CFLAGS = -I$(top_srcdir)/src

ACLOCAL_AMFLAGS = -I m4

lib_LIBRARIES = libnovemberizing.a

libnovemberizing_a_SOURCES = x/std.c                                  \
                             x/sync.c                                 \
                             x/stream.c                               \
                             x/sync/mutex.c                           \
                             x/descriptor.c                           \
                             x/socket.c                               \
                             x/client/socket.c                        \
                             x/session/socket.c                       \
                             x/session/socket/pool.c                  \
                             x/server/socket.c                        \
                             x/client.c                               \
                             x/server.c                               \
                             x/event/engine.c

LIBS = libnovemberizing.a                                             \
       -lpthread

nobase_include_HEADERS = x/std.h                                      \
                         x/sync.h                                     \
                         x/stream.h                                   \
                         x/descriptor.h                               \
                         x/socket.h                                   \
                         x/client/socket.h                            \
                         x/session/socket.h                           \
                         x/session/socket/pool.h                      \
                         x/server/socket.h                            \
                         x/client.h                                   \
                         x/server.h                                   \
                         x/event/engine.h

noinst_PROGRAMS = ExampleSync                                         \
                  ExampleClientSocket                                 \
                  ExampleClientSocketNonblock                         \
                  ExampleClient                                       \
                  ExampleClientNonblock                               \
                  ExampleServerSocket

ExampleServerSocket_SOURCES = example/ExampleServerSocket.c
ExampleServerSocket_LDADD = $(LIBS)

ExampleSync_SOURCES = example/ExampleSync.c
ExampleSync_LDADD   = $(LIBS)

ExampleClientSocket_SOURCES = example/ExampleClientSocket.c
ExampleClientSocket_LDADD   = $(LIBS)

ExampleClientSocketNonblock_SOURCES = example/ExampleClientSocketNonblock.c
ExampleClientSocketNonblock_LDADD   = $(LIBS)

ExampleClient_SOURCES = example/ExampleClient.c
ExampleClient_LDADD = $(LIBS)

ExampleClientNonblock_SOURCES = example/ExampleClientNonblock.c
ExampleClientNonblock_LDADD = $(LIBS)

# noinst_PROGRAMS = SimpleEventEngine                                   \
#                   SimpleConsoleIn                                     \
#                   SimpleConsoleInRead                                 \
#                   SimpleConsoleInReadAll                              \
#                   ExampleXStream                                      \
#                   ExampleXString                                      \
#                   ExampleXConsoleIn                                   \
#                   ExampleXTime                                        \
#                   ExampleXSync                                        \
#                   ExampleXEvent                                       \
#                   ExampleXEventQueue                                  \
#                   ExampleXServer                                      \
#                   ExampleXContainer                                   \
#                   ExampleXList


# ExampleXList_SOURCES               = example/ExampleXList.c
# ExampleXList_LDADD                 = $(LIBS)

# ExampleXContainer_SOURCES          = example/ExampleXContainer.c
# ExampleXContainer_LDADD            = $(LIBS)

# ExampleXServer_SOURCES             = example/ExampleXServer.c
# ExampleXServer_LDADD               = $(LIBS)

# ExampleXEventQueue_SOURCES         = example/ExampleXEventQueue.c
# ExampleXEventQueue_LDADD           = $(LIBS)

# ExampleXEvent_SOURCES              = example/ExampleXEvent.c
# ExampleXEvent_LDADD                = $(LIBS)

# ExampleXSync_SOURCES               = example/ExampleXSync.c
# ExampleXSync_LDADD                 = $(LIBS)

# ExampleXTime_SOURCES               = example/ExampleXTime.c
# ExampleXTime_LDADD                 = $(LIBS)

# SimpleEventEngine_SOURCES          = example/SimpleEventEngine.c
# SimpleEventEngine_LDADD            = $(LIBS)

# SimpleConsoleIn_SOURCES            = example/SimpleConsoleIn.c
# SimpleConsoleIn_LDADD              = $(LIBS)

# SimpleConsoleInRead_SOURCES        = example/SimpleConsoleInRead.c
# SimpleConsoleInRead_LDADD          = $(LIBS)

# SimpleConsoleInReadAll_SOURCES     = example/SimpleConsoleInReadall.c
# SimpleConsoleInReadAll_LDADD       = $(LIBS)

# ExampleXStream_SOURCES             = example/ExampleXStream.c
# ExampleXStream_LDADD               = $(LIBS)

# ExampleXString_SOURCES             = example/ExampleXString.c
# ExampleXString_LDADD               = $(LIBS)

# ExampleXConsoleIn_SOURCES          = example/ExampleXConsoleIn.c
# ExampleXConsoleIn_LDADD            = $(LIBS)


# noinst_PROGRAMS = xmain

# xmain_SOURCES                  = main.c
# xmain_LDADD                    = $(LIBS)

# xeventengine_SOURCES           = event/engine.c
# xeventengine_LDADD             = $(LIBS)
# xeventcommand_SOURCES          = event/command.c
# xeventcommand_LDADD            = $(LIBS)
# xcommand_SOURCES               = command.c
# xcommand_LDADD                 = $(LIBS)
# example_event_queue_SOURCES = example/event/queue.c
# example_event_queue_LDADD   = $(LIBS) libnovemberizing-ex.a
# example_event_queue_rem_SOURCES = example/event/queue/rem.c
# example_event_queue_rem_LDADD   = $(LIBS) libnovemberizing-ex.a
# example_event_engine_SOURCES    = example/event/engine.c
# example_event_engine_LDADD      = $(LIBS)
# example_sync_SOURCES               = example/sync.c
# example_sync_LDADD                 = $(LIBS)
# example_socket_SOURCES      = example/x/socket.c
# example_socket_LDADD        = $(LIBS)
# example_server_SOURCES      = example/x/server.c
# example_server_LDADD        = $(LIBS)


                         
