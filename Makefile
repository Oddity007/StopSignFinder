#blah
all:
	c++ -std=c++11 main.cpp -o findstopsigns -L"/usr/lib/x86_64-linux-gnu" -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_features2d -lopencv_calib3d

test:
	./findstopsigns 

clean:
	rm -f findstopsigns

