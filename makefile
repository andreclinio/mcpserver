include ./definitions.mk

CPPFLAGS := -I$(INCDIR) -I$(SRCDIR) -I$(SRCDIR)/mongoose $(CPPFLAGS)
LDFLAGS := -rcs
LD := ar

LIBFILE := $(LIBDIR)/$(LIBNAME)

OBJS = \
  $(OBJDIR)/mongoose/mongoose.o \
  $(OBJDIR)/mcpserver.o \
  $(OBJDIR)/mcproute.o \
  $(OBJDIR)/mcputils.o \

build: echo-version setup extra $(LIBFILE)

$(LIBFILE): $(OBJS)
	$(LD) $(LDFLAGS) $@  $^ 

extra:
	mkdir -p $(OBJDIR)/mongoose

echo-version: 
	@(echo `grep MCP_LIBRARY_VERSION $(INCDIR)/version.hpp | sed "s/.*\"\(.*\)\"/\1/g"`)


conan-install-test: echo-version
	@(rm -fr $(AUXDIR))
	conan source  . --source-folder=$(AUXDIR)
	conan install . --install-folder=$(AUXDIR)
	conan build   . --source-folder=$(AUXDIR) --build-folder=$(AUXDIR)
	conan package . --source-folder=$(AUXDIR) --build-folder=$(AUXDIR) --package-folder=$(AUXDIR)/package
	@(true rm -fr $(AUXDIR))

conan-install-local: echo-version
	@(rm -fr $(AUXDIR))
	conan source  . --source-folder=$(AUXDIR)
	conan install . --install-folder=$(AUXDIR)
	conan build   . --source-folder=$(AUXDIR) --build-folder=$(AUXDIR)
	conan package . --source-folder=$(AUXDIR) --build-folder=$(AUXDIR) 
	conan package . --source-folder=$(AUXDIR) --build-folder=$(AUXDIR) --package-folder=$(AUXDIR)/package
	conan export-pkg . --package-folder=$(AUXDIR)/package
	
include ./rules.mk
