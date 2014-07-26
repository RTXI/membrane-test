PLUGIN_NAME = membrane_test

HEADERS = membrane-test.h

LIBS = -lgsl -lgslcblas

SOURCES = membrane-test.cpp \
	moc_membrane-test.cpp 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
