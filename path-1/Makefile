OS := $(shell uname)

ifeq ($(OS), Darwin)
	# CLIBS= 		-framework OpenGL -framework GLUT # 2014
	# CFLAGS=		$(CLIBS)
else # Linux
	# CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	# CPATHS=		-I/usr/include -L/usr/lib # 2014
	# CFLAGS=		$(CPATHS)$(CLIBS)
endif

sources = source/driver.cpp source/Board.cpp source/GameController.cpp source/GameTreeManager.cpp source/NetManager.cpp source/NeuralNet.cpp
headers = headers/Board.h headers/GameController.h headers/GameTreeManager.h headers/NetManager.h headers/NeuralNet.h

2048:		$(headers) $(sources)
			g++ -std=c++11 $(sources) -o 2048

2048f:		$(headers) $(sources)
			g++ -std=c++11 -O3 $(sources) -o 2048f

clean:
			rm *.o
