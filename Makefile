CC	= gcc
CFLAGS	= -O0 -Wall
SUBDIR	= Linked-List
LIBS	= -L $(CURDIR)/$(SUBDIR)
MYLIBS	= -llinked_list
SERVER_APPLICATION	= run_server_application
CLIENT_APPLICATION	= run_client_application
SYSTEM_COMPONENTS = TcpClientDBManager.c  TcpClientServiceManager.c \
			TcpNewConnectionAcceptor.c  TcpServerController.c
OBJ_SYSTEM_COMPONENTS = TcpClientDBManager.o  TcpClientServiceManager.o \
			TcpNewConnectionAcceptor.o  TcpServerController.o

all: linked_list.a $(SERVER_APPLICATION) $(CLIENT_APPLICATION)

linked_list.a:
	for dir in $(SUBDIR); do make -C $$dir; done

$(OBJ_SYSTEM_COMPONENTS):
	for src in $(SYSTEM_COMPONENTS); do $(CC) $(CFLAGS) $$src -c; done

$(SERVER_APPLICATION): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) $(LIBS) $(MYLIBS) server_application.c $^ -o $@

$(CLIENT_APPLICATION): $(CLIENT_APPLICATION)
	$(CC) $(CFLAGS) $(LIBS) $(MYLIBS) client_application.c $^ -o $@

.phony: clean
# .phony: clean test

clean:
	for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
	rm -rf *.o $(SERVER_APPLICATION) $(CLIENT_APPLICATION)

#test:
#	@./$(SERVER_APPLICATION) &> /dev/null && echo "Success if zero >>> $$?"
