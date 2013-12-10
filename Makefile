UNAME := $(shell uname)

all: backgammon

backgammon: *.cpp
	g++ -o bkgm *.cpp

debug: *.cpp
	g++ -g -o bkgm *.cpp

clean:
ifeq ($(UNAME), Linux)
	rm bkgm
else
	rm bkgm.exe
endif
