.PHONY: build test clean

build:
	bash build.sh
	
# run: build
# 	cd ./bin && ./turing "$(file)" "$(str)" && cd ..
#
# help: build
# 	bash run.sh -h

test: build
	@./bin/test_statement_parser

clean:
	rm -rf ./bin
	rm -rf ./build