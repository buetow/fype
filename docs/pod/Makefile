# THIS MAKEFILE MAY WORK ONLY WITH (NET)BSD MAKE AKA PMAKE!

all: man html latex txt
html:
	pod2html fype.pod > fype.html
latex:
	pod2latex fype.pod > fype.tex
man:
	pod2man --release "`cat ../version.txt`" --center "The Fype Users Manual Page" fype.pod > fype.man
	cp fype.man fype.1
	if [ -f fype.1.gz ]; then rm fype.1.gz; fi
	gzip fype.1
txt:
	pod2text fype.pod | tee ../../README > fype.txt
pdf:
	pod2pdf --footer-text="This document describes `cat ../version.txt`" fype.pod > fype.pdf
clean:
	for i in html tex man 1.gz pdf; do \
		if [ -f $$i ]; then rm $$i; fi \
	done
