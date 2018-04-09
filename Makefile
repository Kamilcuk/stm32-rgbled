export VERBOSE=1 
all:
	cmake -H. -BBuild
	$(MAKE) -CBuild
.PHONY: Debug Release RelWithDebInfo MinSizeRel
Debug Release RelWithDebInfo MinSizeRel:
	cmake -H. -BBuild -DCMAKE_BUILD_TYPE=$@
	$(MAKE) -CBuild
.PHONY: clean
clean:
	rm -rf Build


