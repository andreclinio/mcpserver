
include ../definitions.mk

CPPFLAGS := -I$(INCDIR) -I$(SRCDIR) -I$(SRCDIR)/mongoose $(CPPFLAGS)
LDFLAGS := -rcs
LD := ar

LIBFILE := $(LIBDIR)/$(LIBNAME)

OBJS = \
  $(OBJDIR)/mongoose/mongoose.o \
  $(OBJDIR)/mcpserver.o \
  $(OBJDIR)/mcproute.o \
  $(OBJDIR)/mcputils.o \

build: setup extra $(LIBFILE)

$(LIBFILE): $(OBJS)
	$(LD) $(LDFLAGS) $@ $^ 

extra:
	mkdir -p $(OBJDIR)/mongoose
