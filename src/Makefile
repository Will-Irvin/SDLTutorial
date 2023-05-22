CC= g++
CCFLAGS= -g -std=c++17 -Wall -Werror
LINKER= -lSDL2 -lSDL2_image
LTEXT= LTexture

TUT1= hello_SDL
TUT2= image_on_screen
TUT4= key_presses
TUT5= opt_surface_and_stretching
TUT6= load_other_image
TUT7= textures
TUT8= geometry
TUT9= viewport
TUT10= color_key
TUT11= clips_and_sprites

TUTALL= $(TUT1) $(TUT2) $(TUT4) $(TUT5) $(TUT6) $(TUT7) $(TUT8) $(TUT9) $(TUT10) $(TUT11)

all: $(TUTALL)

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

$(TUT7): $(TUT7).cc
	$(CC) $(CCFLAGS) $(TUT7).cc $(LINKER) -o $(TUT7)

$(TUT8): $(TUT8).cc
	$(CC) $(CCFLAGS) $(TUT8).cc $(LINKER) -o $(TUT8)

$(TUT9): $(TUT9).cc
	$(CC) $(CCFLAGS) $(TUT9).cc $(LINKER) -o $(TUT9)

$(TUT10): $(TUT10).cc
	$(CC) $(CCFLAGS) $(TUT10).cc $(LINKER) -o $(TUT10)

$(TUT11): $(TUT11).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT11).o $(LTEXT).o $(LINKER) -o $(TUT11)

$(TUT11).o: $(TUT11).cc
	$(CC) $(CCFLAGS) $(TUT11).cc -c

$(LTEXT).o: $(LTEXT).cc
	$(CC) $(CCFLAGS) $(LTEXT).cc -c

clean:
	rm $(TUTALL) $(TUT11).o $(LTEXT).o
