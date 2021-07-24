# Used by Jenkins for automation purposes to build projects, run tests,
# gather test results, build documentation.

ROOT := $(realpath ../..)

# Directories to store results of tests and docs
TEST_RESULTS := $(ROOT)/test_results
DOC_RESULTS := $(ROOT)/doc_results

# Directory with auxiliary scripts
WORKFLOWS := $(ROOT)/tools/workflows

# Defines what targets to build in makefile based projects, the order matter.
TARGETS := all clean

# Find all projects to build
makefile_dirs := $(addprefix $(ROOT)/,demos test testex testhal testrt)
makefiles += $(shell find $(makefile_dirs) -type f -not -path "*/multi/*" \
			   -name Makefile )
multi_makefiles := $(shell find $(makefile_dirs) -type f -path "*/multi/*" \
				     -name "*.make")
external := $(addprefix $(ROOT)/ext/,fatfs lwip pico-sdk wolfssl)

docs := $(shell find $(ROOT)/doc -type f -name Doxyfile_html)
linters := $(shell find $(ROOT)/tools/style -type f -name "*.sh")

.PHONY : all makefiles external docs info lint clean
.PHONY : $(makefiles) $(multi_makefiles)
.PHONY : $(linters) $(docs)

# Build makefile based projects
all : makefiles
makefiles : $(makefiles) $(multi_makefiles)
	@! grep -q -r -m 1 "failure" $(TEST_RESULTS) 2>&1 > /dev/null || ( \
		echo "Exiting because there are build failures"; \
		exit 1)

$(makefiles) : $(external)
	@mkdir -p $(TEST_RESULTS)/$(subst $(ROOT)/,,$(@D))
	+python $(WORKFLOWS)/make.py -C $(@D) -f $(@F) -p $(ROOT)/ \
		-r $(TEST_RESULTS) -s $(WORKFLOWS)/skip.yaml $(TARGETS)

$(multi_makefiles) : $(external)
	@mkdir -p $(TEST_RESULTS)/$(subst $(ROOT)/,,$(@D))
	+python $(WORKFLOWS)/make.py -C $(dir $(@D)) -f make/$(@F) -p $(ROOT)/ \
		-r $(TEST_RESULTS) -s $(WORKFLOWS)/skip.yaml $(TARGETS)

# External projects
external : $(external)
$(external) :
	7z x $@*.7z -o$(@D)

# Build documentation
docs : $(docs)
$(docs) : %Doxyfile_html:
	@mkdir -p $(DOC_RESULTS)
	cd $(@D) && doxygen $(@F)
	tar -czf $(DOC_RESULTS)/$(subst /,_,$(subst $(ROOT)/,,$(@D)))_html.tar.gz \
		--transform "s/^\./$(subst /,_,$(subst $(ROOT)/,,$(@D)))_html/g" \
		-C $(@D)/html .

# Linter checks
lint : $(linters)
	@! grep -r -m 1 ^error: $(TEST_RESULTS)/lint 2>&1 > /dev/null || ( \
		echo "Exiting because there are linter errors"; \
		exit 1)

$(linters) :
	@mkdir -p $(TEST_RESULTS)/lint
	cd $(ROOT)/tools/style && bash $(@F) 2>&1 | \
		tee $(TEST_RESULTS)/lint/$(basename $(@F)).log

# Print versions of used tools
info :
	@which make && make -v
	@echo
	@which ccache && ccache -V || echo "ccache not present"
	@echo
	@which arm-none-eabi-gcc && arm-none-eabi-gcc -v || \
		echo "arm-none-eabi-gcc not present"
	@echo
	@which avr-gcc && avr-gcc -v || echo "avr-gcc not present"
	@echo
	@which doxygen && doxygen -v || echo "doxygen not present"
	@echo
	@which dot && dot -V || echo "graphviz not present"
	@echo
	@which perl && perl -v || echo "perl not present"
	@echo

# Clean up results of tests and docs
clean :
	rm -rf $(TEST_RESULTS)
	rm -rf $(DOC_RESULTS)
	rm -rf $(external)
