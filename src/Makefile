LIB=-L/usr/local/lib
INC=-I/usr/local/include
main:	EagleVision.cpp
	g++ -std=c++11 EagleVision.cpp -o EagleVision -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -lntcore
clean:
	-rm -f EagleVision.o
	-rm -f EagleVision
