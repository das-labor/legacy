
OBJ = example.o gui_lib/gui.o gui_lib/gui_button.o gui_lib/gui_container.o gui_lib/rectangle.o gui_lib/list.o graphics.o fonts/font_uni53.o virtual_display.o main_window.o menu_browser.o touchscreen.o icons/icons.c lap_button.o netvar/netvar.o

test: $(OBJ)
	gcc -enable-stdcall-fixup -o $@ $^ -lsdl



clean:
	rm *.o rm gui_lib/*.o rm fonts/*.o

%.o: %.c
	gcc -Wall -c -o $@ $^

