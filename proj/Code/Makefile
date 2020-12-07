PROG=proj

SRCS = proj.c video.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
#-D __LCOM_OPTIMIZED__
CPPFLAGS += -pedantic  -DPROJ

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
