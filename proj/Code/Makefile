# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c
SRCS += common/utils.c common/interrupts.c common/counters.c
SRCS += video/video.c video/extImages.c
SRCS += kbc/i8042.c
SRCS += kbc/keyboard.c
SRCS += kbc/mouse.c
SRCS += timer/i8254.c
SRCS += rtc/rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DPROJ
#CPPFLAGS += -D __LCOM_OPTIMIZED__

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# object files linking
common/utils.o: common/utils.c common/utils.h
	cc $(CFLAGS) -o common/utils.o -c common/utils.c

common/counters.o: common/counters.c common/counters.h
	cc $(CFLAGS) -o common/counters.o -c common/counters.c

common/interrupts.o: common/interrupts.c common/interrupts.h
	cc $(CFLAGS) -o common/interrupts.o -c common/interrupts.c

video/video.o: video/video.c video/video.h
	cc $(CFLAGS) -o video/video.o -c video/video.c

video/extImages.o: video/extImages.c video/extImages.h
	cc $(CFLAGS) -o video/extImages.o -c video/extImages.c

kbc/i8042.o: kbc/i8042.c kbc/i8042.h
	cc $(CFLAGS) -o kbc/i8042.o -c kbc/i8042.c

kbc/keyboard.o: kbc/keyboard.c kbc/keyboard.h
	cc $(CFLAGS) -o kbc/keyboard.o -c kbc/keyboard.c

kbc/mouse.o: kbc/mouse.c kbc/mouse.h
	cc $(CFLAGS) -o kbc/mouse.o -c kbc/mouse.c

timer/i8254.o: timer/i8254.c timer/i8254.h
	cc $(CFLAGS) -o timer/i8254.o -c timer/i8254.c

rtc/rtc.o: rtc/rtc.c rtc/rtc.h
	cc $(CFLAGS) -o rtc/rtc.o -c rtc/rtc.c

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
