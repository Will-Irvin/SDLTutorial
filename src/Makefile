CC= g++
CCFLAGS= -g -std=c++17 -Wall -Werror
LINKER= -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LTEXT= LTexture
LTIME= LTimer
LWIN= LWindow
DOT= Dot
PAR= Particle
TIL= Tile

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
TUT25= cap_fps
TUT26= motion
TUT27= collision
TUT28= per-pix_collision
TUT29= circ_collision
TUT30= scrolling
TUT31= inf_scrolling
TUT32= text_inp
TUT33= file_io
TUT35= window_events
TUT36= multiple_windows
TUT37= multiple_displays
TUT38= particles
TUT39= tiling
TUT40= texture_manipulation
TUT41= bitmap_fonts
TUT42= texture_stream
TUT43= texture_render
TUT44= frame_ind_move
TUT45= timer_callback

TUTALL= $(TUT1) $(TUT2) $(TUT4) $(TUT5) $(TUT6) $(TUT7) $(TUT8) $(TUT9) $(TUT10) $(TUT11) $(TUT12) $(TUT13) $(TUT14) $(TUT15) $(TUT16) $(TUT17) $(TUT18) $(TUT19) $(TUT21) $(TUT22) $(TUT23) $(TUT24) $(TUT25) $(TUT26) $(TUT27) $(TUT28) $(TUT29) $(TUT30) $(TUT31) $(TUT32) $(TUT33) $(TUT35) $(TUT36) $(TUT37) $(TUT38) $(TUT39) $(TUT40) $(TUT41) $(TUT42) $(TUT43) $(TUT44) $(TUT45)

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

$(TUT25): $(TUT25).o $(LTEXT).o $(LTIME).o
	$(CC) $(CCFLAGS) $(TUT25).o $(LTEXT).o $(LTIME).o $(LINKER) -o $(TUT25)

$(TUT25).o: $(TUT25).cc
	$(CC) $(CCFLAGS) $(TUT25).cc -c

$(TUT26): $(TUT26).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT26).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT26)

$(TUT26).o: $(TUT26).cc
	$(CC) $(CCFLAGS) $(TUT26).cc -c

$(TUT27): $(TUT27).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT27).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT27)

$(TUT27).o: $(TUT27).cc
	$(CC) $(CCFLAGS) $(TUT27).cc -c

$(TUT28): $(TUT28).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT28).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT28)

$(TUT28).o: $(TUT28).cc
	$(CC) $(CCFLAGS) $(TUT28).cc -c

$(TUT29): $(TUT29).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT29).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT29)

$(TUT29).o: $(TUT29).cc
	$(CC) $(CCFLAGS) $(TUT29).cc -c

$(TUT30): $(TUT30).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT30).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT30)

$(TUT30).o: $(TUT30).cc
	$(CC) $(CCFLAGS) $(TUT30).cc -c

$(TUT31): $(TUT31).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT31).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT31)

$(TUT31).o: $(TUT31).cc
	$(CC) $(CCFLAGS) $(TUT31).cc -c

$(TUT32): $(TUT32).o $(LTEXT).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT32).o $(LTEXT).o $(DOT).o $(LINKER) -o $(TUT32)

$(TUT32).o: $(TUT32).cc
	$(CC) $(CCFLAGS) $(TUT32).cc -c

$(TUT33): $(TUT33).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT33).o $(LTEXT).o $(LINKER) -o $(TUT33)

$(TUT33).o: $(TUT33).cc
	$(CC) $(CCFLAGS) $(TUT33).cc -c

$(TUT35): $(TUT35).o $(LTEXT).o $(LWIN).o
	$(CC) $(CCFLAGS) $(TUT35).o $(LTEXT).o $(LWIN).o $(LINKER) -o $(TUT35)

$(TUT35).o: $(TUT35).cc
	$(CC) $(CCFLAGS) $(TUT35).cc -c

$(TUT36): $(TUT36).o $(LTEXT).o $(LWIN).o
	$(CC) $(CCFLAGS) $(TUT36).o $(LTEXT).o $(LWIN).o $(LINKER) -o $(TUT36)

$(TUT36).o: $(TUT36).cc
	$(CC) $(CCFLAGS) $(TUT36).cc -c

$(TUT37): $(TUT37).o $(LTEXT).o $(LWIN).o
	$(CC) $(CCFLAGS) $(TUT37).o $(LTEXT).o $(LWIN).o $(LINKER) -o $(TUT37)

$(TUT37).o: $(TUT37).cc
	$(CC) $(CCFLAGS) $(TUT37).cc -c

$(TUT38): $(TUT38).o $(LTEXT).o $(PAR).o
	$(CC) $(CCFLAGS) $(TUT38).o $(LTEXT).o $(PAR).o $(LINKER) -o $(TUT38)

$(TUT38).o: $(TUT38).cc
	$(CC) $(CCFLAGS) $(TUT38).cc -c

$(TUT39): $(TUT39).o $(LTEXT).o $(TIL).o
	$(CC) $(CCFLAGS) $(TUT39).o $(LTEXT).o $(TIL).o $(LINKER) -o $(TUT39)

$(TUT39).o: $(TUT39).cc
	$(CC) $(CCFLAGS) $(TUT39).cc -c

$(TUT40): $(TUT40).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT40).o $(LTEXT).o $(LINKER) -o $(TUT40)

$(TUT40).o: $(TUT40).cc
	$(CC) $(CCFLAGS) $(TUT40).cc -c

$(TUT41): $(TUT41).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT41).o $(LTEXT).o $(LINKER) -o $(TUT41)

$(TUT41).o: $(TUT41).cc
	$(CC) $(CCFLAGS) $(TUT41).cc -c

$(TUT42): $(TUT42).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT42).o $(LTEXT).o $(LINKER) -o $(TUT42)

$(TUT42).o: $(TUT42).cc
	$(CC) $(CCFLAGS) $(TUT42).cc -c

$(TUT43): $(TUT43).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT43).o $(LTEXT).o $(LINKER) -o $(TUT43)

$(TUT43).o: $(TUT43).cc
	$(CC) $(CCFLAGS) $(TUT43).cc -c

$(TUT44): $(TUT44).o $(LTEXT).o $(LTIME).o $(DOT).o
	$(CC) $(CCFLAGS) $(TUT44).o $(LTEXT).o $(DOT).o $(LTIME).o $(LINKER) -o $(TUT44)

$(TUT44).o: $(TUT44).cc
	$(CC) $(CCFLAGS) $(TUT44).cc -c

$(TUT45): $(TUT45).o $(LTEXT).o
	$(CC) $(CCFLAGS) $(TUT45).o $(LTEXT).o $(LINKER) -o $(TUT45)

$(TUT45).o: $(TUT45).cc
	$(CC) $(CCFLAGS) $(TUT45).cc -c

$(LTEXT).o: $(LTEXT).cc
	$(CC) $(CCFLAGS) $(LTEXT).cc -c

$(LTIME).o: $(LTIME).cc
	$(CC) $(CCFLAGS) $(LTIME).cc -c

$(DOT).o: $(DOT).cc
	$(CC) $(CCFLAGS) $(DOT).cc -c

$(LWIN).o: $(LWIN).cc
	$(CC) $(CCFLAGS) $(LWIN).cc -c

$(PAR).o: $(PAR).cc
	$(CC) $(CCFLAGS) $(PAR).cc -c

$(TIL).o: $(TIL).cc
	$(CC) $(CCFLAGS) $(TIL).cc -c

clean:
	rm $(TUTALL) *.o
