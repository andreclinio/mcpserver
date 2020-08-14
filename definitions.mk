
CCPP := g++
CPPFLAGS := -g -std=c++11


INCDIR := include
SRCDIR := src
OBJDIR := obj
LIBDIR := lib
BINDIR := bin

LIBNAME := libmcpserver.a


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

main: build

clean: 
	rm -fr $(OBJDIR) $(BINDIR) $(LIBDIR)

setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)

