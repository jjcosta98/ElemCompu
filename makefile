FLAGS0 = -O0
FLAGS1 = -O2
FLAGS2 = -O3
FLAGS3 = -Og

OBJECTS1 = heatE.c tools.c
OBJECTS2 = heatI.c tools.c

BIN1 = solheatE
BIN2 = solheatI
BIN3 = soldebugE
BIN4 = soldebugI
BIN5 = solprof

all: $(BIN1)

solheatE: $(OBJECTS1)
	gcc -o $(BIN1) $(FLAGS1) $(OBJECTS1) -lm

solheatI: $(OBJECTS2)
	gcc -o $(BIN2) $(FLAGS1) $(OBJECTS2) -lm

soldebugE: $(OBJECTS1) 
	gcc -g -o $(BIN3) $(FLAGS0) $(OBJECTS1) -lm

soldebugI: $(OBJECTS2) 
	gcc -g -o $(BIN4) $(FLAGS0) $(OBJECTS2) -lm

solprof: $(OBJECTS1)
	gcc -pg -o $(BIN5) $(FLAGS3) $(OBJECTS1) -lm