build_dir := ../build

$(build_dir)/zatacka: main.cpp port.cpp
	mkdir -p $(build_dir)
	g++ -o $(build_dir)/$@ $^

.PHONY: clean

clean:
	rm -rf $(build_dir)
