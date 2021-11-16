build_dir := ../build

$(build_dir)/zatacka: *.cpp *.h
	mkdir -p $(build_dir)
	g++ -o $(build_dir)/$@ *.cpp `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer

.PHONY: clean

clean:
	rm -rf $(build_dir)
