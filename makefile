FLAGS0 = -O0
FLAGS1 = -O2
FLAGS2 = -O3
FLAGS3 = -Og

OBJECTS1 = heatE.c tools.c
OBJECTS2 = heatI.c tools.c

BIN1 = solheatE
BIN2 = solheatI
BIN3 = soldebug
BIN4 = solprof

all: $(BIN1)

solheatE: $(OBJECTS1)
	gcc -o $(BIN1) $(FLAGS1) $(OBJECTS1) -lm

solheatI: $(OBJECTS2)
	gcc -o $(BIN1) $(FLAGS1) $(OBJECTS2) -lm

soldebug: $(OBJECTS1) 
	gcc -g -o $(BIN2) $(FLAGS0) $(OBJECTS1) -lm
  
solprof: $(OBJECTS1)
	gcc -pg -o $(BIN3) $(FLAGS3) $(OBJECTS1) -lm


