
CCPP = g++
CPPFLAGS = -I./inc -I./src/library -I./src/library/mongoose -g -std=c++11
SRCDIR = src
OBJDIR = obj
BINDIR = bin
BINNAME = mcpdemo
 
OBJS = \
  $(OBJDIR)/library/mongoose/mongoose.o \
  $(OBJDIR)/library/macserver.o \
  $(OBJDIR)/library/macroute.o \
  $(OBJDIR)/library/utils.o \
  $(OBJDIR)/sample/mcpdemo.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

$(BINDIR)/$(BINNAME): $(OBJS)
	$(CCPP) $(CPPFLAGS) -o $@ $^ 

clean: 
	rm -fr $(OBJS) $(BINDIR)/$(BINNAME)
