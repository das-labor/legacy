AM_CFLAGS = @GLIB_CFLAGS@

bookmarksdir = $(libdir)/dillo/dpi/bookmarks
downloadsdir = $(libdir)/dillo/dpi/downloads
ftpdir = $(libdir)/dillo/dpi/ftp
httpsdir = $(libdir)/dillo/dpi/https
hellodir = $(libdir)/dillo/dpi/hello
filedir = $(libdir)/dillo/dpi/file
bookmarks_PROGRAMS = bookmarks.dpi
downloads_PROGRAMS = downloads.dpi
ftp_PROGRAMS = ftp.filter.dpi
https_PROGRAMS = https.filter.dpi
hello_PROGRAMS = hello.filter.dpi
file_PROGRAMS = file.dpi

bookmarks_dpi_LDADD = @GLIB_LIBS@
downloads_dpi_LDADD = @GLIB_LIBS@
ftp_filter_dpi_LDADD = @GLIB_LIBS@
https_filter_dpi_LDADD = @GLIB_LIBS@ @LIBSSL_LIBS@
hello_filter_dpi_LDADD = @GLIB_LIBS@
file_dpi_LDADD = @GLIB_LIBS@ @LIBPTHREAD_LIBS@

file_dpi_LDFLAGS = @LIBPTHREAD_LDFLAGS@

bookmarks_dpi_SOURCES = bookmarks.c dpiutil.c dpiutil.h
downloads_dpi_SOURCES = downloads.c dpiutil.c dpiutil.h
ftp_filter_dpi_SOURCES = ftp.c dpiutil.c dpiutil.h
https_filter_dpi_SOURCES = https.c dpiutil.c dpiutil.h
hello_filter_dpi_SOURCES = hello.c dpiutil.c dpiutil.h
file_dpi_SOURCES = file.c dpiutil.c dpiutil.h
