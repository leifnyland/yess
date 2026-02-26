#unit makefile (complete)

CC = g++
INC = ../inc
BIN = ../bin
SRC = ../src
OBJD = ../obj
CFLAGS = -g -c -Wall -std=c++11 -I $(INC)

#lab4 object files whose .C is in the SRC directory
LAB4SRCOBJS = $(OBJD)/Memory.o $(OBJD)/Tools.o $(OBJD)/RegisterFile.o \
 $(OBJD)/ConditionCodes.o

#lab4 object files whose .C is in the unit directory
LAB4UNITOBJS = $(OBJD)/lab4.o $(OBJD)/MemoryTester.o \
 $(OBJD)/RegisterFileTester.o $(OBJD)/ConditionCodesTester.o

#lab5 object files whose .C is in the SRC directory
LAB5SRCOBJS = $(OBJD)/String.o

#lab5 object files whose .C is in the unit directory
LAB5UNITOBJS = $(OBJD)/lab5.o

$(OBJD)/%.o: %.C
	@mkdir -p $(OBJD)
	$(CC) $(CFLAGS) $< -o $@

help:
	@echo "This makefile can be used to compile and execute unit tests."
	@echo "Type either 'make runlab4' OR 'make runlab5'"
	@echo "To just compile the files in this directory type 'make compile'"
	@echo "To just compile the files for lab4 type 'make lab4'"
	@echo "To just compile the files for lab5 type 'make lab5'"

runlab4:
	make lab4
	$(BIN)/lab4

lab4:
	make $(LAB4UNITOBJS)
	(cd $(SRC) && make $(LAB4SRCOBJS))
	$(CC) $(LAB4UNITOBJS) $(LAB4SRCOBJS) -o $(BIN)/lab4

runlab5:
	make lab5
	$(BIN)/lab5

lab5:
	make $(LAB5UNITOBJS)
	(cd $(SRC) && make $(LAB5SRCOBJS))
	$(CC) $(LAB5UNITOBJS) $(LAB5SRCOBJS) -o $(BIN)/lab5


compile: $(LAB4UNITOBJS) $(LAB5UNITOBJS)

$(OBJD)/lab5.o: $(INC)/String.h

$(OBJD)/lab4.o: $(INC)/MemoryTester.h $(INC)/Memory.h \
                $(INC)/RegisterFileTester.h $(INC)/RegisterFile.h \
                $(INC)/ConditionCodesTester.h $(INC)/ConditionCodes.h

$(OBJD)/MemoryTester.o: $(INC)/MemoryTester.h $(INC)/Memory.h

$(OBJD)/RegisterFileTester.o: $(INC)/RegisterFile.h $(INC)/RegisterFileTester.h

$(OBJD)/ConditionCodesTester.o: $(INC)/ConditionCodes.h $(INC)/ConditionCodesTester.h

clean:
	rm -f ../bin/lab4
	rm -f ../bin/lab5
	rm -f $(LAB4UNITOBJS) $(LAB4SRCOBJS) $(LAB5UNITOBJS) $(LAB5SRCOBJS)

