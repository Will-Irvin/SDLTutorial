CC= g++
CCFLAGS= -g -std=c++17 -Wall -Werror
LINKER= -lSDL2
TUT1= hello_SDL
TUT2= image_on_screen
TUT4= key_presses

all: $(TUT1) $(TUT2) $(TUT4)

$(TUT1): $(TUT1).cc
	$(CC) $(CCFLAGS) $(TUT1).cc $(LINKER) -o $(TUT1)

$(TUT2): $(TUT2).cc
	$(CC) $(CCFLAGS) $(TUT2).cc $(LINKER) -o $(TUT2)

$(TUT4): $(TUT4).cc
	$(CC) $(CCFLAGS) $(TUT4).cc $(LINKER) -o $(TUT4)
