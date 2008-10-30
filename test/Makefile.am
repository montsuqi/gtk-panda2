INCLUDES = $(GTK_PANDA_CFLAGS) $(POPPLER_CFLAGS) -I$(top_srcdir)

AM_CPPFLAGS = -g -DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = $(GTK_PANDA_LIBS) $(POPPLER_LIBS) 

if POPPLER
poppler_test = testPoppler
testPoppler_SOURCES = testPoppler.c
endif

bin_PROGRAMS = \
	testGtkButton 				\
	testGtkToggleButton 		\
	testGtkCheckButton 			\
	testGtkRadioButton 			\
	testGtkCalendar 			\
	testGtkEntry 				\
	testGtkLabel 				\
	testGtkFrame 				\
	testGtkList 				\
	testGtkProgressBar 			\
	testGtkNotebook 			\
	testGtkScrolledWindow 		\
	testGnomeFileEntry 			\
	testGnomePixmap 			\
	testGtkTreeView 			\
	testGtkFixed 				\
	testGtkImage 				\
	testGtkFileChooserWidget 	\
	testGtkMozEmbed 			\
	$(poppler_test)

testGtkButton_SOURCES = testGtkButton.c
testGtkToggleButton_SOURCES = testGtkToggleButton.c
testGtkCheckButton_SOURCES = testGtkCheckButton.c
testGtkRadioButton_SOURCES = testGtkRadioButton.c
testGtkCalendar_SOURCES = testGtkCalendar.c
testGtkEntry_SOURCES = testGtkEntry.c
testGtkLabel_SOURCES = testGtkLabel.c
testGtkFrame_SOURCES = testGtkFrame.c
testGtkList_SOURCES  = testGtkList.c
testGtkProgressBar_SOURCES = testGtkProgressBar.c
testGtkNotebook_SOURCES = testGtkNotebook.c
testGtkScrolledWindow_SOURCES = testGtkScrolledWindow.c
testGnomeFileEntry_SOURCES = testGnomeFileEntry.c
testGnomePixmap_SOURCES = testGnomePixmap.c
testGtkTreeView_SOURCES = testGtkTreeView.c
testGtkFixed_SOURCES = testGtkFixed.c
testGtkImage_SOURCES = testGtkImage.c
testGtkFileChooserWidget_SOURCES = testGtkFileChooserWidget.c
testGtkMozEmbed_SOURCES = testGtkMozEmbed.c
