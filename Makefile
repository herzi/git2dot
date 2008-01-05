all: git2dot

git2dot: main.c
	gcc -o $@ $^ $(shell pkg-config --cflags --libs glib-2.0)
