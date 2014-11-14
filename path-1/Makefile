OS := $(shell uname)

out_dir = bin
src_dir = source
h_dir = headers

libs = $(out_dir)/Board.lib $(out_dir)/GameController.lib $(out_dir)/GameTreeManager.lib $(out_dir)/NetManager.lib $(out_dir)/NeuralNet.lib

ifeq ($(OS), Darwin) # OSX
	CLIBS= 		-std=c++11
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-std=c++11 # Untested
	CPATHS=		-I/usr/include -L/usr/lib
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

# Compile any lib
$(out_dir)/%.lib: $(src_dir)/%.cpp $(h_dir)/%.h
	mkdir -p $(out_dir)
	g++ -c $< -o $@

# Compile 2048
2048: $(libs) $(src_dir)/driver.cpp
	g++ $(CFLAGS) $(src_dir)/driver.cpp $(libs) -o $(out_dir)/2048

# Compile 2048 with compiler optimization flags to make it faster
2048f: $(libs) $(src_dir)/driver.cpp
	g++ -O3 $(CFLAGS) $(src_dir)/driver.cpp $(libs) -o $(out_dir)/2048f

# Clean up
clean:
		rm $(out_dir)/*
