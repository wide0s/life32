PROJ=a.out

SRCS = main.c
OBJS = ${SRCS:.c=.o}

all: ${PROJ}

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

${PROJ}: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) -f ${PROJ} $(OBJS) $(OBJS:%.o=%.d)

-include $(OBJS:%.o=%.d)
