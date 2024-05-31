CC	= gcc
CFLAGS	= -O0 -Wall

# Dependent libraries
SUBDIR_ll	= Linked-List
SUBDIR_cbb	= CircularByteBuffer
SUBDIRS	= $(SUBDIR_ll) $(SUBDIR_cbb)
LIB_ll	= -L $(CURDIR)/$(SUBDIR_ll)
LIB_cbb	= -L $(CURDIR)/$(SUBDIR_cbb)
LIBS	= $(LIB_ll) $(LIB_cbb)
MYLIBS	= -llinked_list -lcbb

# Executables
BASIC_UTIL_TEST		= exec_util_test
BASIC_ECHO_CLIENT	= exec_echo_client
BASIC_ECHO_SERVER	= exec_echo_server
SERVER_APPLICATION	= run_server_application
CLIENT_APPLICATION	= run_client_application

# Sources
SYSTEM_COMPONENTS = TcpClientDBManager.c  TcpClientServiceManager.c \
			TcpNewConnectionAcceptor.c  TcpServerController.c TcpClient.c \
			TcpMessageDemarcation.c TcpClientUtil.c
OBJ_SYSTEM_COMPONENTS = TcpClientDBManager.o  TcpClientServiceManager.o \
			TcpNewConnectionAcceptor.o  TcpServerController.o TcpClient.o \
			TcpMessageDemarcation.o TcpClientUtil.o

all: libraries $(SERVER_APPLICATION) $(CLIENT_APPLICATION) $(BASIC_UTIL_TEST) \
	$(BASIC_ECHO_SERVER) $(BASIC_ECHO_CLIENT)

libraries:
	for dir in $(SUBDIRS); do make -C $$dir; done

$(OBJ_SYSTEM_COMPONENTS):
	for src in $(SYSTEM_COMPONENTS); do $(CC) $(CFLAGS) $$src -c; done

$(SERVER_APPLICATION): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) $(LIBS) $(MYLIBS) tests/server_application.c $^ -o tests/$@

$(CLIENT_APPLICATION): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) tests/client_application.c TcpClientUtil.o -o tests/$@

$(BASIC_UTIL_TEST): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) tests/test_TcpClientUtil.c TcpClientUtil.o -o tests/$@

$(BASIC_ECHO_SERVER): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) $(LIBS) $(MYLIBS) echo_server/echo_server.c $^ -o echo_server/$@

$(BASIC_ECHO_CLIENT): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) echo_server/echo_client.c TcpClientUtil.o -o echo_server/$@

.phony: clean test

clean:
	for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
	rm -rf *.o tests/$(SERVER_APPLICATION) tests/$(CLIENT_APPLICATION) \
		tests/$(BASIC_UTIL_TEST) echo_server/$(BASIC_ECHO_CLIENT) echo_server/$(BASIC_ECHO_SERVER)

test: $(BASIC_UTIL_TEST)
	@./tests/$(BASIC_UTIL_TEST) &> /dev/null && echo "Success when zero >>> $$?"
