CC	= gcc
CFLAGS	= -O0 -Wall
SUBDIR_ll	= Linked-List
SUBDIR_cbb	= CircularByteBuffer
SUBDIRS	= $(SUBDIR_ll) $(SUBDIR_cbb)
LIB_ll	= -L $(CURDIR)/$(SUBDIR_ll)
LIB_cbb	= -L $(CURDIR)/$(SUBDIR_cbb)
LIBS	= $(LIB_ll) $(LIB_cbb)
MYLIBS	= -llinked_list -lcbb
BASIC_UTIL_TEST		= exec_util_test
SERVER_APPLICATION	= run_server_application
CLIENT_APPLICATION	= run_client_application
SYSTEM_COMPONENTS = TcpClientDBManager.c  TcpClientServiceManager.c \
			TcpNewConnectionAcceptor.c  TcpServerController.c TcpClient.c \
			TcpMessageDemarcation.c TcpClientUtil.c
OBJ_SYSTEM_COMPONENTS = TcpClientDBManager.o  TcpClientServiceManager.o \
			TcpNewConnectionAcceptor.o  TcpServerController.o TcpClient.o \
			TcpMessageDemarcation.o TcpClientUtil.o

all: libraries $(SERVER_APPLICATION) $(CLIENT_APPLICATION) $(BASIC_UTIL_TEST)

libraries:
	for dir in $(SUBDIRS); do make -C $$dir; done

$(OBJ_SYSTEM_COMPONENTS):
	for src in $(SYSTEM_COMPONENTS); do $(CC) $(CFLAGS) $$src -c; done

$(SERVER_APPLICATION): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) $(LIBS) $(MYLIBS) server_application.c $^ -o $@

$(CLIENT_APPLICATION): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) client_application.c TcpClientUtil.o -o $@

$(BASIC_UTIL_TEST): $(OBJ_SYSTEM_COMPONENTS)
	$(CC) $(CFLAGS) test_TcpClientUtil.c TcpClientUtil.o -o $@

.phony: clean test

clean:
	for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
	rm -rf *.o $(SERVER_APPLICATION) $(CLIENT_APPLICATION) $(BASIC_UTIL_TEST)

test: $(BASIC_UTIL_TEST)
	@./$(BASIC_UTIL_TEST) &> /dev/null && echo "Success if zero >>> $$?"
