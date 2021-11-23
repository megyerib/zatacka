build_dir := build
debug_dir := $(build_dir)/debug
release_dir := $(build_dir)/release
deploy_dir := deploy

packer := $(build_dir)/packer

sdl_flags := `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer

release: $(release_dir)/zatacka

debug: $(debug_dir)/zatacka

run: release
	$(release_dir)/zatacka

gdb: debug
	gdb $(debug_dir)/zatacka

deploy: $(deploy_dir)/zatacka-linux.zip

$(debug_dir)/zatacka: code/*.cpp code/*.h Makefile $(build_dir)/resource.h
	mkdir -p $(debug_dir)
	g++ \
		-o $@ \
		-Og -g3 \
		code/*.cpp \
		$(build_dir)/resource.c -I$(build_dir) \
		$(sdl_flags)

$(release_dir)/zatacka: code/*.cpp code/*.h Makefile $(build_dir)/resource.h
	mkdir -p $(release_dir)
	g++ \
		-o $@ \
		-O3 \
		code/*.cpp \
		$(build_dir)/resource.c -I$(build_dir) \
		$(sdl_flags)

$(deploy_dir)/zatacka-linux.zip: $(release_dir)/zatacka
	mkdir -p $(deploy_dir)
	rm -f $@
	zip -j $@ $^

$(build_dir)/resource.h: $(packer) res/death.png res/logo.png res/font_hun.ttf res/font_bold_hun.ttf
	$(packer)

$(packer): packer/main.c packer/packer.c packer/packer.h Makefile
	mkdir -p $(build_dir)
	gcc -o $@ packer/*.c

.PHONY: clean

clean:
	rm -rf $(build_dir)
	rm -rf $(deploy_dir)
