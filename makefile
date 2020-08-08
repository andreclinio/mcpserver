
CCPP = g++
CPPFLAGS = -I./inc -I./src/library -I./src/library/mongoose
SRC = src
DEPS = server/server.cpp 
OBJS = \
  $(SRC)/library/mongoose/mongoose.o \
  $(SRC)/library/macserver.o \
  $(SRC)/library/macroute.o \
  $(SRC)/library/utils.o \
  $(SRC)/server/server.o

%.o: %.c $(DEPS)
	echo "Comp $< ...."
	$(CCPP) -c -o $@ $< $(CFLAGS)

server: $(OBJS)
	$(CCPP) -o $@ $^ $(CFLAGS)

clean: 
	rm -fr $(OBJS)
