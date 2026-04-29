CPP = g++
OPTIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++13


main.exe: main.o
	$(CPP) -o main.exe main.o

main.o: main.cc
	$(CPP) -c main.cc $(OPTIONS)

clean:
	rm *.o
	rm *.exe