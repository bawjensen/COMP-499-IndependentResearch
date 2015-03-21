OS := $(shell uname)

# Input and output directories
out_dir = bin
src_dir = source
h_dir = headers

# Places to search for sources
VPATH = $(src_dir):$(h_dir)

# Names of the libraries
projs = Board GameController GameTreeManager NetManager NeuralNet RandomGen
libs = $(foreach proj,$(projs),$(out_dir)/$(proj).lib)
libsf = $(foreach proj,$(projs),$(out_dir)/$(proj)f.lib)

ifeq ($(OS), Darwin) # OSX
	CLIBS= 		-std=c++11
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-std=c++11 # Untested
	CPATHS=		-I/usr/include -L/usr/lib
	CFLAGS=		$(CPATHS) $(CLIBS)
endif

# Compile any lib
$(out_dir)/%f.lib: %.cpp %.h
	@mkdir -p $(out_dir) # @ prefix silences the command
	g++ -O3 $(CFLAGS) -c $< -o $@

# Compile any lib
$(out_dir)/%.lib: %.cpp %.h
	@mkdir -p $(out_dir) # @ prefix silences the command
	g++ $(CFLAGS) -c $< -o $@

# Compile 2048-trainer
2048-trainer-b: $(libs) $(src_dir)/trainer.cpp
	g++ $(CFLAGS) $(src_dir)/trainer.cpp $(libs) -o $(out_dir)/2048-trainer

# Compile 2048-trainer with compiler optimization flags to make it faster
2048-trainer-f-b: $(libsf) $(src_dir)/trainer.cpp
	g++ -O3 $(CFLAGS) $(src_dir)/trainer.cpp $(libsf) -o $(out_dir)/2048-trainer-f

# Compile 2048-trainer
2048-runner-b: $(libs) $(src_dir)/runner.cpp
	g++ $(CFLAGS) $(src_dir)/runner.cpp $(libs) -o $(out_dir)/2048-runner

# Compile 2048-runner with compiler optimization flags to make it faster
2048-runner-f-b: $(libsf) $(src_dir)/runner.cpp
	g++ -O3 $(CFLAGS) $(src_dir)/runner.cpp $(libsf) -o $(out_dir)/2048-runner-f

# Clean up
clean:
	rm $(out_dir)/*
