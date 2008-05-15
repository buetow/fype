all:
	cd ../ && yasm -f elf ./tmp/test2.s -o ./tmp/test2.o && \
	cc -lc ./tmp/test2.o -o ./tmp/test2 
run: all
	./test2
clean:
	for i in test2.o test2; do \
		if [ -f $$i ]; then rm $$i; fi; \
	done
	
