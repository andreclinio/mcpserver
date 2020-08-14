
CCPP = g++
CPPFLAGS = -I./inc -I./src/library -I./src/library/mongoose -g -std=c++11
SRCDIR = src
OBJDIR = obj
BINDIR = bin
BINNAME = mcpdemo
 
OBJS = \
  $(OBJDIR)/library/mongoose/mongoose.o \
  $(OBJDIR)/library/mcpserver.o \
  $(OBJDIR)/library/mcproute.o \
  $(OBJDIR)/library/mcputils.o \
  $(OBJDIR)/sample/mcpdemo.o

main: setup $(BINDIR)/$(BINNAME)

rebuild: clean main

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

$(BINDIR)/$(BINNAME): $(OBJS)
	$(CCPP) $(CPPFLAGS) -o $@ $^ 

clean: 
	rm -fr $(OBJDIR) $(BINDIR)

setup:
	mkdir -p $(OBJDIR)/library 
	mkdir -p $(OBJDIR)/library/mongoose
	mkdir -p $(OBJDIR)/sample
	mkdir -p $(BINDIR)
