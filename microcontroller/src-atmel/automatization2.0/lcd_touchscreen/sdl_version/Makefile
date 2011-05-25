
OBJ = pc/main.o pc/virtual_display.o pc/touchscreen.o can_pc/can.o can_pc/cann.o can_pc/debug.o can_pc/can-encap.o pc/graphics.o \
      gui_lib/gui.o gui_lib/gui_button.o gui_lib/gui_container.o gui_lib/rectangle.o gui_lib/gui_graph_view.o \
      gui_lib/fonts/font_uni53.o gui_lib/icons/icons.o util_lib/list.o \
      main_window.o menu_browser.o lap_button.o netvar/netvar.o can_handler.o

test: $(OBJ)
	gcc -enable-stdcall-fixup -o $@ $^ -lsdl -lwsock32



clean:
	rm -f *.o gui_lib/*.o gui_lib/fonts/*.o gui_lib/icons/*.o can_pc/*.o netvar/*.o pc/*.o util_lib/*.o

%.o: %.c
	gcc -Wall -g -c -o $@ $^

