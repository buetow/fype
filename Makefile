# THIS MAKEFILE ONLY WORKS WITH (NET)BSD MAKE AKA PMAKE!

BIN=fype
SRCS!=find ./src -name '*.c'
OBJS=$(SRCS:.c=.o)
CC?=cc
#CC=mingw32-gcc
DEBUG=-g3 -ggdb3
CFLAGS+=-c -Wall -std=c99 -pedantic $(DEBUG)
LDADD+=
HEADER?=docs/header.txt
OSYSTEM!=uname
PREFIX=/usr/local
all: build $(OBJS) newline stats-tofile	
	@$(CC) -lm -o $(BIN) $(OBJS) $(LDADD) 	
	@if test -z '$(DEBUG)'; then strip $(BIN) ; fi	
	@awk '$$2 == "BUILDNR" { printf("===> Fype build number % 13s :% 6s%d\n", \
		"", "", $$3); exit(0); }' src/build.h
	@echo "===> Fype binary size			:     `du -hs $(BIN)`"	
	@#echo "===> `./$(BIN) -v | sed 's/Build .*//'`"
	@./fype -v > ./docs/version.txt
	@./fype -h > ./docs/help.txt
	@./fype -s > ./docs/synopses.txt
	@echo
$(OBJS):
	$(CC) $(LDADD) $(CFLAGS) `echo $@ | sed 's/\.o/\.c/'` -o $@
clean:
	find ./ -name 1 -exec rm -f {} \;
	find ./ -name '*.o' -exec rm -f {} \;
	find ./ -name '*.bin' -exec rm -f {} \;
	find . -name '*.core' -exec rm -f {} \;
	if [ -f $(BIN) ]; then rm -f $(BIN); fi
build:	ctags
	@awk '{ \
		if ($$2 == "BUILDNR") print $$1,$$2,$$3+1; \
		else if ($$2 ~ /OS_/) printf("%s OS_%s\n", $$1, \
			toupper("$(OSYSTEM)")); \
		else print }' \
			src/build.h  >.tmp && mv -f .tmp src/build.h
printbuild:
	@awk '$$2 == "BUILDNR" { printf("%d\n", \
		$$3); exit(0); }' src/build.h
ctags:
	@# Generating Source-Tags for Vim 
	ctags `find . -name '*.c'`
style: astyle check
astyle:
	find ./src -name '*.[ch]' -exec sh -c 'astyle -s3 {}; rm -f {}.orig' \;
check:
	for f in `find ./src -name '*.[ch]'`; do awk -v f=$$f \
		'{ if (length($$0) > 80) { \
		printf "Max line length reached @ %s:%d => %d\n", \
		f, NR, length($$0) } }' $$f; done
touch:
	find ./src -name '*.c' -exec touch {} \;
stats:
	@sh -c 'wc=`find ./src -name "*.[ch]" | xargs wc -l`; \
		echo "===> Num of C source files		: `echo \"$$wc\" | \
		grep -E \"\\.(c|h)$$\" | wc -l`"; \
		echo "===> Num of C source lines		: `echo \"$$wc\" | \
		tail -n 1 | sed s/total//`"'
	@sh -c 'wc=`find ./examples -name "*.fy" | xargs wc -l`; \
		echo "===> Num of Fype source examples	: `echo \"$$wc\" | \
		grep -E \"\\.fy$$\" | wc -l`"; \
		echo "===> Num of Fype source lines		: `echo \"$$wc\" | \
		tail -n 1 | sed s/total//`"'
stats-tofile:
	make stats | tee ./docs/stats.txt
testrun:
	cat ./tmp/test.fy > ./tmp/test.out
	./$(BIN) -V ./tmp/test.fy | tee -a ./tmp/test.out
tr: testrun
test: all testrun
t: test
run:
	./$(BIN) ./tmp/test.fy
core:
	gdb $(BIN) $(BIN).core
gdb:
	gdb --args $(BIN) ./tmp/test.fy
newline:
	@echo 
examples: all
	echo > ./examples/all-examples.txt
	for i in ./examples/*.fy; do \
		echo "===> Running $$i"; \
		./$(BIN) $$i; \
		cat $$i >> ./examples/all-examples.txt; \
		echo >> ./examples/all-examples.txt; \
	done
replace:
	find ./src -name '*.[ch]' -exec sh -c 'sed -n "s/$(FROM)/$(INTO)/g; \
		w .tmp" {} && mv -f .tmp {}' \;
headers:
	@find ./src -name '*.[ch]' -exec sh -c 'export FILE={}; \
		make header' \;
header:
	@echo "===> Processing $(FILE)"
	@sed -n '/*:/d; w .tmp' $(FILE) 
	@header=`sed 's/\(.*\)/ echo " \*: \1"/' $(HEADER)`; \
		echo '/*:*' > $(FILE); eval "$$header" >> $(FILE); \
		echo ' *:*/' >> $(FILE); cat .tmp >> $(FILE); rm -f .tmp
tar: clean
	sh -c 'build=`make printbuild`;cd ../;\
		tar cvjf $(BIN)-$$build.tar.bz2 $(BIN)'
install: all
	cp ./$(BIN) $(PREFIX)/bin
	cp ./docs/pod/fype.1.gz $(PREFIX)/man/man1
deinstall:
	rm $(PREFIX)/bin/$(BIN)
	rm $(PREFIX)/man/man1/fype.1.gz
uninstall: deinstall
pod:
	@cd ./docs/pod; make clean all
