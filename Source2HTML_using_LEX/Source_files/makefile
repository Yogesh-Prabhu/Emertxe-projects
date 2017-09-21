all:
	lex lex_prog.l
	gcc lex.yy.c -ll source2html.c
	./a.out c_src_file.c dest_file.html

clean:
	rm a.out lex.yy.c dest_file.html
