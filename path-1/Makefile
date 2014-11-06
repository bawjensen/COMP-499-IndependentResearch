OS := $(shell uname)

ifeq ($(OS), Darwin)
	# CLIBS= 		-framework OpenGL -framework GLUT # 2014
	# CFLAGS=		$(CLIBS)
else # Linux
	# CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	# CPATHS=		-I/usr/include -L/usr/lib # 2014
	# CFLAGS=		$(CPATHS)$(CLIBS)
endif

2048:		source/driver.cpp source/GameController.cpp source/InputHandler.cpp source/Board.cpp headers/Board.h source/NeuralNet.cpp headers/NeuralNet.h
			g++ -std=c++11 source/driver.cpp source/GameController.cpp source/InputHandler.cpp source/Board.cpp source/NeuralNet.cpp -o 2048

2048f:		source/driver.cpp source/GameController.cpp source/InputHandler.cpp source/Board.cpp headers/Board.h source/NeuralNet.cpp headers/NeuralNet.h
			g++ -std=c++11 -O3 source/driver.cpp source/GameController.cpp source/InputHandler.cpp source/Board.cpp source/NeuralNet.cpp -o 2048

clean:
			rm *.o
