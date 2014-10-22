NODE = general.cpp setv.cpp msg.cpp
CPPFLAGS=-O2

all: general

general: $(NODE)
	g++ $(CPPFLAGS) -o ../bin/general $(NODE)

clean:
	rm -f *~
	rm -f ../bin/general

