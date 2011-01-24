
OBJ = main.o gui_lib/gui.o gui_lib/gui_button.o gui_lib/gui_container.o gui_lib/rectangle.o gui_lib/list.o graphics.o gui_lib/fonts/font_uni53.o virtual_display.o main_window.o menu_browser.o touchscreen.o gui_lib/icons/icons.o lap_button.o netvar/netvar.o can_pc/can.o lap_button.o can_pc/cann.o can_pc/debug.o can_pc/can-encap.o can_handler.o

test: $(OBJ)
	gcc -enable-stdcall-fixup -o $@ $^ -lsdl -lwsock32



clean:
	rm *.o gui_lib/*.o gui_lib/fonts/*.o gui_lib/icons/*.o can_pc/*.o

%.o: %.c
	gcc -Wall -c -o $@ $^

