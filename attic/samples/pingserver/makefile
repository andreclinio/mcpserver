
include ../../../definitions.mk

BINNAME := pingserver
CPPFLAGS := -I../../../$(INCDIR) $(CPPFLAGS)
OBJS := $(OBJDIR)/$(BINNAME).o 

build: setup $(BINDIR)/$(BINNAME)

$(BINDIR)/$(BINNAME): $(OBJS)
	$(CCPP) $(CPPFLAGS) -o $@ $^ ../../../lib/$(LIBNAME)

include ../../../rules.mk