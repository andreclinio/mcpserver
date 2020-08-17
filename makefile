
main: build

%:
	@(echo "> LIBRARY $@"; cd library ; make $@ ; cd ..)
	@(echo "> PINGSERVER $@"; cd samples/pingserver ; make $@ ; cd ../..)
	@(echo "> MCPDEMOSERVER $@"; cd samples/mcpdemoserver ; make $@ ; cd ../..)

conan-test:
	rm -fr tmp
	conan source  . --source-folder=tmp/source
	conan install . --install-folder=tmp/build
	conan build   . --source-folder=tmp/source --build-folder=tmp/build
	conan package . --source-folder=tmp/source --build-folder=tmp/build --package-folder=tmp/package