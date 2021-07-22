# Used by Jenkins for automation purposes to build projects, run tests,
# gather test results, build documentation.

ROOT := $(realpath ../..)

# Directories to store results of tests and docs
TEST_RESULTS := $(ROOT)/test_results
DOC_RESULTS := $(ROOT)/doc_results

# Defines what targets to build in makefile based projects, the order matter.
TARGETS := all clean

# Find all projects to build
makefiles := $(shell find $(ROOT)/demos -type f -name Makefile)
makefiles += $(shell find $(ROOT)/test -type f -name Makefile)
makefiles += $(shell find $(ROOT)/testex -type f -name Makefile)
makefiles += $(shell find $(ROOT)/testhal -type f -name Makefile -not \
			   -path "*/multi/*")
makefiles += $(shell find $(ROOT)/testrt -type f -name Makefile)
multi_makefiles := $(shell find $(ROOT)/testhal -type f -path "*/multi/*" \
					 -name "*.make")
docs := $(shell find $(ROOT)/doc -type f -name Doxyfile_html)
linters := $(shell find $(ROOT)/tools/style -type f -name "*.sh")

.PHONY : all makefiles docs info lint clean
.PHONY : $(makefiles) $(multi_makefiles)
.PHONY : $(linters) $(docs)

# Build makefile based projects
all : makefiles
makefiles : $(makefiles) $(multi_makefiles)
	@! grep -q -r -m 1 \\"failure\\" $(TEST_RESULTS) 2>&1 > /dev/null || ( \
		echo "Exiting because there are build failures"; \
		exit 1)

$(makefiles) :
	@mkdir -p $(TEST_RESULTS)/$(subst $(ROOT)/,,$(@D))
	+python $(ROOT)/tools/workflows/make.py -C $(@D) -f $(@F) \
		-p $(ROOT)/ -r $(TEST_RESULTS) $(TARGETS)

$(multi_makefiles) :
	@mkdir -p $(TEST_RESULTS)/$(subst $(ROOT)/,,$(@D))
	+python $(ROOT)/tools/workflows/make.py -C $(dir $(@D)) -f make/$(@F) \
		-p $(ROOT)/ -r $(TEST_RESULTS) $(TARGETS)

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
