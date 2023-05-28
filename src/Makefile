CC= g++
CCFLAGS= -g -std=c++17 -Wall -Werror
LINKER= -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LTEXT= LTexture
LTIME= LTimer

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
TUT12= color_mod
TUT13= alpha_blending
TUT14= animation
TUT15= rotate
TUT16= ttf_text
TUT17= mouse
TUT18= key_state
TUT19= joystick
TUT21= mixer
TUT22= timers
TUT23= adv_timers
TUT24= frame_rate

TUTALL= $(TUT1) $(TUT2) $(TUT4) $(TUT5) $(TUT6) $(TUT7) $(TUT8) $(TUT9) $(TUT10) $(TUT11) $(TUT12) $(TUT13) $(TUT14) $(TUT15) $(TUT16) $(TUT17) $(TUT18) $(TUT19) $(TUT21) $(TUT22) $(TUT23) $(TUT24)

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

$(TUT12): $(TUT12).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT12).o $(LTEXT).o $(LINKER) -o $(TUT12)

$(TUT12).o: $(TUT12).cc
	$(CC) $(CCFLAGS) $(TUT12).cc -c

$(TUT13): $(TUT13).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT13).o $(LTEXT).o $(LINKER) -o $(TUT13)

$(TUT13).o: $(TUT13).cc
	$(CC) $(CCFLAGS) $(TUT13).cc -c

$(TUT14): $(TUT14).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT14).o $(LTEXT).o $(LINKER) -o $(TUT14)

$(TUT14).o: $(TUT14).cc
	$(CC) $(CCFLAGS) $(TUT14).cc -c

$(TUT15): $(TUT15).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT15).o $(LTEXT).o $(LINKER) -o $(TUT15)

$(TUT15).o: $(TUT15).cc
	$(CC) $(CCFLAGS) $(TUT15).cc -c

$(TUT16): $(TUT16).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT16).o $(LTEXT).o $(LINKER) -o $(TUT16)

$(TUT16).o: $(TUT16).cc
	$(CC) $(CCFLAGS) $(TUT16).cc -c

$(TUT17): $(TUT17).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT17).o $(LTEXT).o $(LINKER) -o $(TUT17)

$(TUT17).o: $(TUT17).cc
	$(CC) $(CCFLAGS) $(TUT17).cc -c

$(TUT18): $(TUT18).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT18).o $(LTEXT).o $(LINKER) -o $(TUT18)

$(TUT18).o: $(TUT18).cc
	$(CC) $(CCFLAGS) $(TUT18).cc -c

$(TUT19): $(TUT19).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT19).o $(LTEXT).o $(LINKER) -o $(TUT19)

$(TUT19).o: $(TUT19).cc
	$(CC) $(CCFLAGS) $(TUT19).cc -c

$(TUT21): $(TUT21).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT21).o $(LTEXT).o $(LINKER) -o $(TUT21)

$(TUT21).o: $(TUT21).cc
	$(CC) $(CCFLAGS) $(TUT21).cc -c

$(TUT22): $(TUT22).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT22).o $(LTEXT).o $(LINKER) -o $(TUT22)

$(TUT22).o: $(TUT22).cc
	$(CC) $(CCFLAGS) $(TUT22).cc -c

$(TUT23): $(TUT23).o $(LTEXT).o $(LTIME).o
	$(CC) $(CCFLAGS) $(TUT23).o $(LTEXT).o $(LTIME).o $(LINKER) -o $(TUT23)

$(TUT23).o: $(TUT23).cc
	$(CC) $(CCFLAGS) $(TUT23).cc -c

$(TUT24): $(TUT24).o $(LTEXT).o $(LTIME).o
	$(CC) $(CCFLAGS) $(TUT24).o $(LTEXT).o $(LTIME).o $(LINKER) -o $(TUT24)

$(TUT24).o: $(TUT24).cc
	$(CC) $(CCFLAGS) $(TUT24).cc -c

$(LTEXT).o: $(LTEXT).cc
	$(CC) $(CCFLAGS) $(LTEXT).cc -c

$(LTIME).o: $(LTIME).cc
	$(CC) $(CCFLAGS) $(LTIME).cc -c

clean:
	rm $(TUTALL) $(TUT11).o $(TUT12).o $(TUT13).o $(TUT14).o $(TUT15).o $(TUT16).o
	rm $(TUT17).o $(TUT18).o $(TUT19).o $(TUT21).o $(TUT22).o $(TUT23).o $(TUT24).o
	rm $(LTIME).o $(LTEXT).o
