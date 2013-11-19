all: backgammon

backgammon: *.cpp
	g++ -o bkgm *.cpp
clean:
	rm 
