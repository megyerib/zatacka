build_dir := ../build
flags := -Og -g3

$(build_dir)/zatacka: *.cpp *.h Makefile
	mkdir -p $(build_dir)
	g++ -o $(build_dir)/$@ $(flags) *.cpp `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer

.PHONY: clean

clean:
	rm -rf $(build_dir)
