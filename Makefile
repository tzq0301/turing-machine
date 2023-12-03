.PHONY: build run clean

build:
	cmake -B ./build && cd ./build && make && cd ..
	
run: build
	cd ./bin && ./turing && cd ..
	
clean:
	rm -rf ./bin
	rm -rf ./build