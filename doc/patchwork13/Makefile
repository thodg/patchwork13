
SOURCES = 	index.sgml \
		bookinfo.sgml \
		glossary.sgml \
		gui.sgml \
		index.sgml \
		install.sgml \
		pw13.sgml

all: all-html

all-html: ${SOURCES}
	docbook2html -o html $<

clean:
	rm -rf html 2>/dev/null || true
