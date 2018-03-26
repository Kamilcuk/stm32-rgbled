proj:=rgbled
BUILD_TYPE:=RELEASE

all: release

release: %: Build/%/$(proj).elf
release: BUILD_TYPE:=RELEASE
debug:   %: Build/%/$(proj).elf
debug:   BUILD_TYPE:=DEBUG

.SECONDARY:

.PHONY: FORCE
FORCE:

Build/%/$(proj).elf: FORCE
	@mkdir -p $(dir $@)
	cmake -H. -B$(dir $@) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -C $(dir $@)

.PHONY: clean
clean:
	rm -rf Build/$*


