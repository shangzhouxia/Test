.PHONY: proj clean

test:
	protoc --cpp_out=. test.proto 
	export PKG_CONFIG_PATH=/usr/local/protobuf/lib/pkgconfig
	g++ -std=c++11 test.pb.cc test.cpp -o test `pkg-config --cflags --libs protobuf`

proj:
	protoc --cpp_out=. proj.proto 
	export PKG_CONFIG_PATH=/usr/local/protobuf/lib/pkgconfig
	g++ -std=c++11 proj.pb.cc proj.cpp -o proj `pkg-config --cflags --libs protobuf`

clean:
	rm *.pb.*
	rm test
	rm proj
