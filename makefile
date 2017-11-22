CFLAGS = -Wall -O3 -g -mavx -march=native -DLIKWID_PERFMON
#-I/home/soft/likwid/include/ -L/home/soft/likwid/lib/
LDLIBS = -lm -llikwid
OBJECTS = core.o utils.o entrada_saida.o

invmat: $(OBJECTS)

doc:
	doxygen config
	ln -s doc/html/index.html doc.html

cdoc:
	rm -rf doc/ doc.html

clean:
	rm -f *.o

purge:
	make clean
	rm -f invmat

teste:
	make clean
	make
	./invmat -i 10 -r 5
