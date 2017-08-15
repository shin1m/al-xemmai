xemmaidir = $(XEMMAI_MODULE_PATH)

xemmai_LTLIBRARIES = al.la
AM_CPPFLAGS = $(OPENAL_CFLAGS) $(ALUT_CFLAGS) $(XEMMAI_CFLAGS)
AM_CXXFLAGS =
AM_LDFLAGS = -module -avoid-version
if DEBUG
AM_CXXFLAGS += -g
else
AM_CPPFLAGS += -DNDEBUG
AM_CXXFLAGS += -O3
endif
if PROFILE
AM_CXXFLAGS += -pg
AM_LDFLAGS += -pg
endif
al_la_LIBADD = $(OPENAL_LIBS) $(ALUT_LIBS) $(XEMMAI_LIBS)
al_la_SOURCES = \
	cast.h \
	error.h \
	error.cc \
	device.h \
	device.cc \
	context.h \
	context.cc \
	source.h \
	source.cc \
	buffer.h \
	buffer.cc \
	al.h \
	al.cc
