
DIRS := library samples/pingserver samples/mcpdemoserver

main: build

%:
	@(for d in $(DIRS) ; do	echo $$d ; pushd $$d ; make $@ ; popd  ; done)