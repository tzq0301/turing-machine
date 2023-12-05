.PHONY: build run clean

build:
	bash build.sh
	
# run: build
# 	cd ./bin && ./turing "$(file)" "$(str)" && cd ..
#
# help: build
# 	bash run.sh -h
	
clean:
	rm -rf ./bin
	rm -rf ./build