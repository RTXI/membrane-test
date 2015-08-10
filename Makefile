PLUGIN_NAME = membrane_test

HEADERS = membrane-test.h

SOURCES = membrane-test.cpp \
          moc_membrane-test.cpp 

LIBS = -lgsl -lgslcblas

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
