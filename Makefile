build_dir := build
debug_dir := $(build_dir)/debug
release_dir := $(build_dir)/release

sdl_flags := `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer

release: $(release_dir)/zatacka

debug: $(debug_dir)/zatacka

run: release
	$(release_dir)/zatacka

gdb: debug
	gdb $(debug_dir)/zatacka

$(debug_dir)/zatacka: code/*.cpp code/*.h Makefile
	mkdir -p $(debug_dir)
	g++ -o $@ -Og -g3 code/*.cpp $(sdl_flags)

$(release_dir)/zatacka: code/*.cpp code/*.h Makefile
	mkdir -p $(release_dir)
	g++ -o $@ -O3 code/*.cpp $(sdl_flags)

.PHONY: clean

clean:
	rm -rf $(build_dir)
