AM_CFLAGS = -I$(top_srcdir)/src

ACLOCAL_AMFLAGS = -I m4

lib_LIBRARIES = libnovemberizing.a

libnovemberizing_a_SOURCES = x/std.c                                  \
                             x/object.c                               \
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
                             x/session.c                              \
                             x/session/pool.c                         \
                             x/event.c                                \
                             x/event/queue.c                          \
                             x/event/pool.c                           \
                             x/event/engine.c                         \
                             x/event/engine/wait.c                    \
                             x/event/generator.c                      \
                             x/event/subscription.c                   \
                             x/command.c                              \
                             x/command/event.c                        \
                             x/command/event/generator.c              \
                             x/command/event/subscription.c

LIBS = libnovemberizing.a                                             \
       -lpthread

nobase_include_HEADERS = x/std.h                                      \
                         x/object.h                                   \
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
                         x/session.h                                  \
                         x/session/pool.h                             \
                         x/event.h                                    \
                         x/event/queue.h                              \
                         x/event/pool.h                               \
                         x/event/engine.h                             \
                         x/event/engine/wait.h                        \
                         x/event/generator.h                          \
                         x/event/subscription.h                       \
                         x/command.h                                  \
                         x/command/event.h                            \
                         x/command/event/generator.h                  \
                         x/command/event/subscription.h

noinst_PROGRAMS = ExampleSync                                         \
                  ExampleClientSocket                                 \
                  ExampleClientSocketNonblock                         \
                  ExampleClient                                       \
                  ExampleClientNonblock                               \
                  ExampleServerSocket                                 \
                  ExampleServerSocketNonblock                         \
                  ExampleServer                                       \
                  ExampleServerNonblock                               \
                  ExampleCommand                                      \
                  ExampleEventEngine                                  \
                  ExampleEventEngineCommand


ExampleEventEngineCommand_SOURCES = example/ExampleEventEngineCommand.c
ExampleEventEngineCommand_LDADD = $(LIBS)

ExampleCommand_SOURCES = example/ExampleCommand.c
ExampleCommand_LDADD = $(LIBS)

ExampleEventEngine_SOURCES = example/ExampleEventEngine.c
ExampleEventEngine_LDADD = $(LIBS)

ExampleServerNonblock_SOURCES = example/ExampleServerNonblock.c
ExampleServerNonblock_LDADD = $(LIBS)

ExampleServer_SOURCES = example/ExampleServer.c
ExampleServer_LDADD = $(LIBS)

ExampleServerSocket_SOURCES = example/ExampleServerSocket.c
ExampleServerSocket_LDADD = $(LIBS)

ExampleServerSocketNonblock_SOURCES = example/ExampleServerSocketNonblock.c
ExampleServerSocketNonblock_LDADD = $(LIBS)

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
