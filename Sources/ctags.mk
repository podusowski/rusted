TARGET:=ctags
TYPE:=custom

ctags:
	@/bin/echo -e '$(FONT_BOLD)ctags$(FONT_RESET)'
	@ctags -R --c++-kinds=+p --fields=+miaS --extra=+q --exclude=".svn" -f tags . /usr/include/c++
