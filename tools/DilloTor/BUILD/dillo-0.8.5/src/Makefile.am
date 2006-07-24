AM_CPPFLAGS=-DDILLORC_SYS='"$(sysconfdir)/dillorc"' @LIBJPEG_CPPFLAGS@
AM_CFLAGS = @GTK_CFLAGS@ @LIBPNG_CFLAGS@

SUBDIRS = IO

bin_PROGRAMS = dillo

dillo_LDADD = IO/libDio.a @LIBJPEG_LIBS@ @LIBPNG_LIBS@ @GTK_LIBS@ @LIBZ_LIBS@ @LIBPTHREAD_LIBS@
dillo_LDFLAGS = @LIBJPEG_LDFLAGS@ @LIBPTHREAD_LDFLAGS@

dillo_SOURCES = \
	msg.h \
	acconfig.h \
	chain.h \
	chain.c \
	commands.c \
	commands.h \
	cache.c	\
	cache.h	\
	capi.c \
	capi.h \
	debug.h \
	dw.h \
	dw.c \
	dw_aligned_page.c \
	dw_aligned_page.h \
	dw_bullet.c \
	dw_bullet.h \
	dw_button.c \
	dw_button.h \
	dw_container.c \
	dw_container.h \
	dw_embed_gtk.c	\
	dw_embed_gtk.h	\
	dw_ext_iterator.h \
	dw_ext_iterator.c \
	dw_gtk_scrolled_frame.c \
	dw_gtk_scrolled_frame.h \
	dw_gtk_scrolled_window.c \
	dw_gtk_scrolled_window.h \
	dw_gtk_statuslabel.c \
	dw_gtk_statuslabel.h \
	dw_gtk_viewport.c \
	dw_gtk_viewport.h \
	dw_hruler.c \
	dw_hruler.h \
	dw_image.c \
	dw_image.h \
	dw_list_item.c \
	dw_list_item.h \
	dw_marshal.c \
	dw_marshal.h \
	dw_page.c \
	dw_page.h \
	dw_style.c \
	dw_style.h \
	dw_table.c \
	dw_table.h \
	dw_table_cell.c \
	dw_table_cell.h \
	dw_tooltip.c \
	dw_tooltip.h \
	dw_widget.c \
	dw_widget.h \
	findtext.c \
	findtext.h \
	selection.c \
	selection.h \
	web.c	\
	web.h	\
	progressbar.c \
	progressbar.h \
	dillo.c \
	dillo.h \
	bookmark.c \
	bookmark.h \
	browser.h \
	dicache.c \
	dicache.h \
	dns.c \
	dns.h \
	gtk_ext_button.c \
	gtk_ext_button.h \
	gtk_ext_menu.c \
	gtk_ext_menu.h \
	gtk_ext_menu_item.c \
	gtk_ext_menu_item.h \
	gtk_menu_title.c \
	gtk_menu_title.h \
	gif.c \
	jpeg.c \
	png.c \
	html.c \
	html.h \
	image.c \
	image.h \
	misc.c \
	misc.h \
	interface.h \
	interface.c \
	history.c \
	history.h \
	nav.c \
	nav.h \
	plain.c \
	menu.c \
	menu.h \
	prefs.c \
	prefs.h \
	colors.c \
	colors.h \
	bitvec.c \
	bitvec.h \
	klist.c \
	klist.h \
	strbuf.c \
	strbuf.h \
	url.c \
	url.h \
	cookies.c \
	cookies.h \
	list.h \
	binaryconst.h \
	pixmaps.h \
	dpiapi.c \
	dpiapi.h

EXTRA_DIST = chg srch
