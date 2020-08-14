
include ../../definitions.mk

BINNAME := pingserver
CPPFLAGS := -I../../library/$(INCDIR) $(CPPFLAGS)
OBJS := $(OBJDIR)/$(BINNAME).o 

build: setup $(BINDIR)/$(BINNAME)

$(BINDIR)/$(BINNAME): $(OBJS)
	$(CCPP) $(CPPFLAGS) -o $@ $^ ../../library/lib/$(LIBNAME)

