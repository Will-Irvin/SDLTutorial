CC= g++
CCFLAGS= -g -std=c++17 -Wall -Werror
LINKER= -lSDL2 -lSDL2_image
TUT1= hello_SDL
TUT2= image_on_screen
TUT4= key_presses
TUT5= opt_surface_and_stretching
TUT6= load_other_image

all: $(TUT1) $(TUT2) $(TUT4) $(TUT5) $(TUT6)

$(TUT1): $(TUT1).cc
	$(CC) $(CCFLAGS) $(TUT1).cc $(LINKER) -o $(TUT1)

$(TUT2): $(TUT2).cc
	$(CC) $(CCFLAGS) $(TUT2).cc $(LINKER) -o $(TUT2)

$(TUT4): $(TUT4).cc
	$(CC) $(CCFLAGS) $(TUT4).cc $(LINKER) -o $(TUT4)

$(TUT5): $(TUT5).cc
	$(CC) $(CCFLAGS) $(TUT5).cc $(LINKER) -o $(TUT5)

$(TUT6): $(TUT6).cc
	$(CC) $(CCFLAGS) $(TUT6).cc $(LINKER) -o $(TUT6)
