CC	= gcc
CFLAGS	= -O0 -Wall
SUBDIR	= Linked-List
LIBS	= -L $(CURDIR)/$(SUBDIR)
MYLIBS	= -llinked_list
APPLICATION	= exec_application
SYSTEM_COMPONENTS = TcpClientDBManager.c  TcpClientServiceManager.c \
			TcpNewConnectionAcceptor.c  TcpServerController.c
OBJ_SYSTEM_COMPONENTS = TcpClientDBManager.o  TcpClientServiceManager.o \
			TcpNewConnectionAcceptor.o  TcpServerController.o

all: linked_list.a $(APPLICATION)

linked_list.a:
	for dir in $(SUBDIR); do make -C $$dir; done

$(OBJ_SYSTEM_COMPONENTS):
	for src in $(SYSTEM_COMPONENTS); do $(CC) $(CFLAGS) $$src -c; done

$(APPLICATION): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) $(LIBS) $(MYLIBS) test_app.c $^ -o $@

clean:
	for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
	rm -rf *.o $(APPLICATION)
