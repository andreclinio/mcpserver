
main: build

%:
	@(echo "> LIBRARY $@"; cd library ; make $@ ; cd ..)
	@(echo "> PINGSERVER $@"; cd samples/pingserver ; make $@ ; cd ../..)
	@(echo "> MCPDEMOSERVER $@"; cd samples/mcpdemoserver ; make $@ ; cd ../..)

build-library:
	@(echo "> LIBRARY BUILD" && cd library && make clean build && cd ..)

conan-install-test:
	rm -fr tmp
	conan source  . --source-folder=tmp/source
	conan install . --install-folder=tmp/build
	conan build   . --source-folder=tmp/source --build-folder=tmp/build
	conan package . --source-folder=tmp/source --build-folder=tmp/build --package-folder=tmp/package

conan-install-local: build-library
	conan export .
