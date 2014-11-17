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
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

# Compile any lib
$(out_dir)/%f.lib: %.cpp %.h
	@mkdir -p $(out_dir) # @ prefix silences the command
	g++ -O3 $(CFLAGS) -c $< -o $@

# Compile any lib
$(out_dir)/%.lib: %.cpp %.h
	@mkdir -p $(out_dir) # @ prefix silences the command
	g++ $(CFLAGS) -c $< -o $@

# Compile 2048
2048-b: $(libs) $(src_dir)/driver.cpp
	g++ $(CFLAGS) $(src_dir)/driver.cpp $(libs) -o $(out_dir)/2048

# Compile 2048
2048-r: 2048-b
	bin/2048

# Compile 2048
2048-rd: 2048-b
	bin/2048 -d

# Compile 2048 with compiler optimization flags to make it faster
2048f-b: $(libsf) $(src_dir)/driver.cpp
	g++ -O3 $(CFLAGS) $(src_dir)/driver.cpp $(libsf) -o $(out_dir)/2048f

2048f-r: 2048f-b
	bin/2048f

2048f-rd: 2048f-b
	bin/2048f -d

# Clean up
clean:
	rm $(out_dir)/*
