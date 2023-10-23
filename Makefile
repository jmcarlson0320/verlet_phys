CC = clang
CFLAGS = -g -Wall -I/Users/jmc/devel/include -I/opt/homebrew/include
LIBS = -L/Users/jmc/devel/lib -L/opt/homebrew/lib -ltinyfw -lm -lsdl2
EXE = app

$(EXE): main.o simulation.o verlet.o vec.o
	$(CC) -o $@ $^ $(LIBS)

run: $(EXE)
	./$(EXE)

main.o: simulation.h
simulation.o: simulation.h verlet.h vec.h
verlet.o: verlet.h vec.h
vec.o: vec.h

clean:
	rm *.o $(EXE)

.PHONY: run clean
