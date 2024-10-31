.PHONY: proj clean

proj:
	export PKG_CONFIG_PATH=/usr/local/protobuf/lib/pkgconfig
	protoc --cpp_out=. proj.proto 
	g++ -std=c++11 proj.pb.cc proj.cpp -o proj `pkg-config --cflags --libs protobuf`

clean:
	rm *.pb.*
	rm test
	rm proj
