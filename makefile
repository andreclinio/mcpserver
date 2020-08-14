
main: build

%:
	@(echo "> libray"; cd library ; make $@ ; cd ..)
	@(echo "> pingserver"; cd samples/pingserver ; make $@ ; cd ../..)
	@(echo "> mcpdemoserver"; cd samples/mcpdemoserver ; make $@ ; cd ../..)