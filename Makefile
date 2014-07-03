#bezier-cv:/mnt/bezier-cv.cpp
#	g++ -O0 -g3`pkg-config --cflags --libs opencv` $< -o $@

%: %.cpp
	rm $@ -f
	g++ `pkg-config --cflags --libs opencv` $< -o $@
