
# Compile: create object files from C source files.
$(OUTPUTDIR)/%.o : %.c
	@echo
	$(CC) -c $(CFLAGS) $< -o $@ 
