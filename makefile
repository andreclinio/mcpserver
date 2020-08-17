
main: build

%:
	@(echo "> LIBRARY $@"; cd library ; make $@ ; cd ..)
	@(echo "> PINGSERVER $@"; cd samples/pingserver ; make $@ ; cd ../..)
	@(echo "> MCPDEMOSERVER $@"; cd samples/mcpdemoserver ; make $@ ; cd ../..)