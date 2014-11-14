OS := $(shell uname)

ifeq ($(OS), Darwin) # OSX
	CLIBS= 		-std=c++11
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-std=c++11 # Untested
	CPATHS=		-I/usr/include -L/usr/lib
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

sources = source/driver.cpp source/Board.cpp source/GameController.cpp source/GameTreeManager.cpp source/NetManager.cpp source/NeuralNet.cpp
headers = headers/Board.h headers/GameController.h headers/GameTreeManager.h headers/NetManager.h headers/NeuralNet.h

2048:		$(headers) $(sources)
			g++ $(CFLAGS) $(sources) -o bin/2048

2048f:		$(headers) $(sources)
			g++ $(CFLAGS) -O3 $(sources) -o bin/2048f

clean:
			rm *.o
