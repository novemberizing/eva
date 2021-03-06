

----

- [ ] 제로 카피는 마스크로 제공하자. (엔진의 프로세스에서 제로 카피 마스크가 존재하면, WRITE, READ, WRITE 로직을 READ 수행 시에 WRITE 를 하도록 하자. SEND FILE 을 수행할 수 있도록 하자.)
- [ ] 이벤트 타입과 디스크립터 상태 상수를 정리하자.
- [ ] 콘솔 I/O 에서 OUT FLUSH 의 경우 - WRITE 를 수행하고
- [ ] WRITE 버퍼가 더 이상 존재하지 않을 경우
- [ ] OUT FLUSH 상태에서만 OUT 이 발생할 수 있도록 하자.

- [x] 콘솔에 EVENT ON 이 존재하지 않아서 SEG FAULT 가 발생한다.
- [x] 메모리 릭 체크를 위해서 콘솔 입출력을 통해서 CLI 를 구현하자.
- [x] 함수에 함수의 입출력에 대한 로그를 삽입하였습니다.
- [x] 로그 함수중에서 FUNCTION START/END 삽입 시에 space padding 을 이용해서 체크하도록 하였습니다.
- [x] 로그 함수중에서 INFINITE 로그가 발생할 경우를 위해서 xdebugonly(code) 를 통해서 출력을 하지 않도록 하였습니다.
- [x] 패딩을 이용해서 로그 타입 문자열의 패딩을 적용할 수 있도록 하자.
- [x] 콘솔 서브스크라이버에 콘솔 디스크립터 파라미터 추가
- [x] 콘솔 아웃 시에 버퍼에만 삽입하고 나머지는 로직에 맞긴다.
- [x] 콘솔에 FLUSH 로직을 적용하자.
- [x] IN FLUSH 의 경우 버퍼를 비우자.
- [x] 스트림 클리어 함수를 만들자.

-----

- [ ] 문서를 정리할 때이다.
- [ ] STRING 관련 함수를 만들자.
- [ ] 정규 표현식 관련하여 POSIX 를 대체할만한 무엇인가를 만들자.

- [x] 메모리 릭 체크

```
novemberizing@novemberizing-switch:~/Workspace/novemberizing/eva$ sudo valgrind --leak-check=full --show-leak-kinds=all ./src/evad 
==31330== Memcheck, a memory error detector
==31330== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==31330== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==31330== Command: ./src/evad
==31330== 
greeting
eva> Quit
==31330== 
==31330== HEAP SUMMARY:
==31330==     in use at exit: 492 bytes in 12 blocks
==31330==   total heap usage: 475 allocs, 463 frees, 277,962 bytes allocated
==31330== 
==31330== 4 bytes in 1 blocks are still reachable in loss record 1 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x10B198: xlogout (log.c:132)
==31330==    by 0x113CE0: xconsolesubscriber_set (console.c:87)
==31330==    by 0x10A7CF: main (eva.c:33)
==31330== 
==31330== 16 bytes in 1 blocks are still reachable in loss record 2 of 12
==31330==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x10AFE7: xlogpath_set (log.c:90)
==31330==    by 0x10A7B6: main (eva.c:30)
==31330== 
==31330== 16 bytes in 1 blocks are still reachable in loss record 3 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x10B1C8: xlogout (log.c:135)
==31330==    by 0x113CE0: xconsolesubscriber_set (console.c:87)
==31330==    by 0x10A7CF: main (eva.c:33)
==31330== 
==31330== 32 bytes in 1 blocks are still reachable in loss record 4 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x119C82: xeventqueue_new (queue.c:26)
==31330==    by 0x10D712: xeventengine_new (engine.c:27)
==31330==    by 0x10A7D4: main (eva.c:34)
==31330== 
==31330== 32 bytes in 1 blocks are still reachable in loss record 5 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x119C82: xeventqueue_new (queue.c:26)
==31330==    by 0x10D71F: xeventengine_new (engine.c:28)
==31330==    by 0x10A7D4: main (eva.c:34)
==31330== 
==31330== 32 bytes in 1 blocks are still reachable in loss record 6 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x11359B: xdescriptoreventgeneratorsubscriptionlist_new (list.c:23)
==31330==    by 0x11237A: xdescriptoreventgenerator_new (epoll.c:266)
==31330==    by 0x10D733: xeventengine_new (engine.c:30)
==31330==    by 0x10A7D4: main (eva.c:34)
==31330== 
==31330== 32 bytes in 1 blocks are still reachable in loss record 7 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x11359B: xdescriptoreventgeneratorsubscriptionlist_new (list.c:23)
==31330==    by 0x112386: xdescriptoreventgenerator_new (epoll.c:267)
==31330==    by 0x10D733: xeventengine_new (engine.c:30)
==31330==    by 0x10A7D4: main (eva.c:34)
==31330== 
==31330== 48 bytes in 1 blocks are still reachable in loss record 8 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x112371: xdescriptoreventgenerator_new (epoll.c:264)
==31330==    by 0x10D733: xeventengine_new (engine.c:30)
==31330==    by 0x10A7D4: main (eva.c:34)
==31330== 
==31330== 64 bytes in 1 blocks are still reachable in loss record 9 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x11A9D2: xeventsubscription_new (subscription.c:17)
==31330==    by 0x10DF71: xeventengine_server_register (engine.c:168)
==31330==    by 0x10A7FA: main (eva.c:36)
==31330== 
==31330== 64 bytes in 1 blocks are still reachable in loss record 10 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x11A9D2: xeventsubscription_new (subscription.c:17)
==31330==    by 0x10E16D: xeventengine_descriptor_register (engine.c:195)
==31330==    by 0x10A814: main (eva.c:37)
==31330== 
==31330== 64 bytes in 1 blocks are still reachable in loss record 11 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x11A9D2: xeventsubscription_new (subscription.c:17)
==31330==    by 0x10E16D: xeventengine_descriptor_register (engine.c:195)
==31330==    by 0x10A82E: main (eva.c:38)
==31330== 
==31330== 88 bytes in 1 blocks are still reachable in loss record 12 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x10D709: xeventengine_new (engine.c:25)
==31330==    by 0x10A7D4: main (eva.c:34)
==31330== 
==31330== LEAK SUMMARY:
==31330==    definitely lost: 0 bytes in 0 blocks
==31330==    indirectly lost: 0 bytes in 0 blocks
==31330==      possibly lost: 0 bytes in 0 blocks
==31330==    still reachable: 492 bytes in 12 blocks
==31330==         suppressed: 0 bytes in 0 blocks
==31330== 
==31330== For lists of detected and suppressed errors, rerun with: -s
==31330== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```




----

1. LEAK = 

```
==31330== 4 bytes in 1 blocks are still reachable in loss record 1 of 12
==31330==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==31330==    by 0x10B198: xlogout (log.c:132)
==31330==    by 0x113CE0: xconsolesubscriber_set (console.c:87)
==31330==    by 0x10A7CF: main (eva.c:33)
```


2. LEAK

```
==32468== 16 bytes in 1 blocks are still reachable in loss record 1 of 11
==32468==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==32468==    by 0x10AFEC: xlogpath_set (log.c:90)
==32468==    by 0x10A7B6: main (eva.c:30)
```

3. LEAK

```
==37270== 32 bytes in 1 blocks are still reachable in loss record 1 of 9
==37270==    at 0x483DD99: calloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==37270==    by 0x119D19: xeventqueue_new (queue.c:26)
==37270==    by 0x10D7A9: xeventengine_new (engine.c:27)
==37270==    by 0x10A7D4: main (eva.c:34)
```

==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FBC02: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x1136D0: xdescriptoreventgeneratorsubscriptionlist_new (list.c:27)
==38351==    by 0x112441: xdescriptoreventgenerator_new (epoll.c:266)
==38351==    by 0x10D7CA: xeventengine_new (engine.c:30)
==38351==    by 0x10A7D4: main (eva.c:34)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FBC02: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x1136D0: xdescriptoreventgeneratorsubscriptionlist_new (list.c:27)
==38351==    by 0x11244D: xdescriptoreventgenerator_new (epoll.c:267)
==38351==    by 0x10D7CA: xeventengine_new (engine.c:30)
==38351==    by 0x10A7D4: main (eva.c:34)
==38351== 
==38351== Use of uninitialised value of size 8
==38351==    at 0x48DF81B: _itoa_word (_itoa.c:179)
==38351==    by 0x48FB6F4: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x116460: xserversocket_new (socket.c:25)
==38351==    by 0x116026: xservernew (server.c:17)
==38351==    by 0x10AC63: evaechoserver_get (echo.c:45)
==38351==    by 0x10A7E5: main (eva.c:36)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48DF82D: _itoa_word (_itoa.c:179)
==38351==    by 0x48FB6F4: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x116460: xserversocket_new (socket.c:25)
==38351==    by 0x116026: xservernew (server.c:17)
==38351==    by 0x10AC63: evaechoserver_get (echo.c:45)
==38351==    by 0x10A7E5: main (eva.c:36)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FC3A8: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x116460: xserversocket_new (socket.c:25)
==38351==    by 0x116026: xservernew (server.c:17)
==38351==    by 0x10AC63: evaechoserver_get (echo.c:45)
==38351==    by 0x10A7E5: main (eva.c:36)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FB86E: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x116460: xserversocket_new (socket.c:25)
==38351==    by 0x116026: xservernew (server.c:17)
==38351==    by 0x10AC63: evaechoserver_get (echo.c:45)
==38351==    by 0x10A7E5: main (eva.c:36)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x483EF58: strlen (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==38351==    by 0x48FCE94: __vfprintf_internal (vfprintf-internal.c:1688)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x10C0CB: xstreamfront (stream.c:127)
==38351==    by 0x115A6E: xconsoleout_flush (console.c:545)
==38351==    by 0x10AA92: evacli (console.c:42)
==38351==    by 0x114D0A: xconsoledescriptorsubscriber_input (console.c:340)
==38351==    by 0x112783: xdescriptoreventgenerator_register (epoll.c:323)
==38351==    by 0x10E25F: xeventengine_descriptor_register (engine.c:202)
==38351==    by 0x10A82E: main (eva.c:38)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FBC02: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x10C0CB: xstreamfront (stream.c:127)
==38351==    by 0x115A6E: xconsoleout_flush (console.c:545)
==38351==    by 0x10AA92: evacli (console.c:42)
==38351==    by 0x114D0A: xconsoledescriptorsubscriber_input (console.c:340)
==38351==    by 0x112783: xdescriptoreventgenerator_register (epoll.c:323)
==38351==    by 0x10E25F: xeventengine_descriptor_register (engine.c:202)
==38351==    by 0x10A82E: main (eva.c:38)
==38351== 
==38351== Use of uninitialised value of size 8
==38351==    at 0x48DF7BA: _itoa_word (_itoa.c:180)
==38351==    by 0x48FB6F4: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x10C0CB: xstreamfront (stream.c:127)
==38351==    by 0x115A6E: xconsoleout_flush (console.c:545)
==38351==    by 0x10AA92: evacli (console.c:42)
==38351==    by 0x114D0A: xconsoledescriptorsubscriber_input (console.c:340)
==38351==    by 0x112783: xdescriptoreventgenerator_register (epoll.c:323)
==38351==    by 0x10E25F: xeventengine_descriptor_register (engine.c:202)
==38351==    by 0x10A82E: main (eva.c:38)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48DF7CC: _itoa_word (_itoa.c:180)
==38351==    by 0x48FB6F4: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x10C0CB: xstreamfront (stream.c:127)
==38351==    by 0x115A6E: xconsoleout_flush (console.c:545)
==38351==    by 0x10AA92: evacli (console.c:42)
==38351==    by 0x114D0A: xconsoledescriptorsubscriber_input (console.c:340)
==38351==    by 0x112783: xdescriptoreventgenerator_register (epoll.c:323)
==38351==    by 0x10E25F: xeventengine_descriptor_register (engine.c:202)
==38351==    by 0x10A82E: main (eva.c:38)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FC3A8: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x10C0CB: xstreamfront (stream.c:127)
==38351==    by 0x115A6E: xconsoleout_flush (console.c:545)
==38351==    by 0x10AA92: evacli (console.c:42)
==38351==    by 0x114D0A: xconsoledescriptorsubscriber_input (console.c:340)
==38351==    by 0x112783: xdescriptoreventgenerator_register (epoll.c:323)
==38351==    by 0x10E25F: xeventengine_descriptor_register (engine.c:202)
==38351==    by 0x10A82E: main (eva.c:38)
==38351== 
==38351== Conditional jump or move depends on uninitialised value(s)
==38351==    at 0x48FB86E: __vfprintf_internal (vfprintf-internal.c:1687)
==38351==    by 0x10B575: xlogout (log.c:224)
==38351==    by 0x10C0CB: xstreamfront (stream.c:127)
==38351==    by 0x115A6E: xconsoleout_flush (console.c:545)
==38351==    by 0x10AA92: evacli (console.c:42)
==38351==    by 0x114D0A: xconsoledescriptorsubscriber_input (console.c:340)
==38351==    by 0x112783: xdescriptoreventgenerator_register (epoll.c:323)
==38351==    by 0x10E25F: xeventengine_descriptor_register (engine.c:202)
==38351==    by 0x10A82E: main (eva.c:38)




==39156== Use of uninitialised value of size 8
==39156==    at 0x48DF81B: _itoa_word (_itoa.c:179)
==39156==    by 0x48FB6F4: __vfprintf_internal (vfprintf-internal.c:1687)
==39156==    by 0x10B575: xlogout (log.c:224)
==39156==    by 0x11646B: xserversocket_new (socket.c:25)
==39156==    by 0x116031: xservernew (server.c:17)
==39156==    by 0x10AC63: evaechoserver_get (echo.c:45)
==39156==    by 0x10A7E5: main (eva.c:36)