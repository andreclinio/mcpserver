
clean: 
	rm -fr $(OBJDIR) $(BINDIR) $(LIBDIR)

setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

rebuild: clean build