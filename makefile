
CCPP = g++
CPPFLAGS = -I./inc -I./src/library -I./src/library/mongoose -g -rdynamic
SRC = src
DEPS = server/server.cpp 
OBJS = \
  $(SRC)/library/mongoose/mongoose.o \
  $(SRC)/library/macserver.o \
  $(SRC)/library/macroute.o \
  $(SRC)/library/utils.o \
  $(SRC)/sample/server.o

%.o: %.c $(DEPS)
	echo "Comp $< ...."
	$(CCPP) -c -o $@ $< $(CFLAGS)

server: $(OBJS)
	$(CCPP) -g -rdynamic -o $@ $^ $(CFLAGS)

clean: 
	rm -fr $(OBJS)
