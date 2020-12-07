#PROG=proj
#
#SRCS = proj.c video.c
#
## __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
##-D __LCOM_OPTIMIZED__
#CPPFLAGS += -pedantic  -DPROJ
#
#DPADD += ${LIBLCF}
#LDADD += -llcf
#
#.include <minix.lcom.mk>

# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c
SRCS += video/video.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DPROJ
#CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# object files linking
video/video.o: video/video.c video/video.h
	cc $(CFLAGS) -o video/video.o -c video/video.c

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
