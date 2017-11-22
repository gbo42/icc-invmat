CFLAGS = -Wall -O3 -mavx -g -march=native -DLIKWID_PERFMON
#-I/home/soft/likwid/include/ -L/home/soft/likwid/lib/
LDLIBS = -lm -llikwid
OBJECTS = core.o utils.o entrada_saida.o

invmat: $(OBJECTS)

doc:
	doxygen config
	ln -s doc/html/index.html doc.html

clean:
	rm -rf *.o doc/ doc.html

purge:
	make clean
	rm -f invmat

teste:
	make clean
	make
	./invmat -i 10 -r 5
