
RESULT = chc_patchwork13.pdf

all: ${RESULT}

${RESULT}: patchwork13.jpg planning.jpg

.PHONY:pub
pub: ${RESULT}
	scp ${RESULT} ssh.sf.net:/home/groups/p/pa/patchwork13/htdocs/doc/

%.pdf: %.tex
	pdflatex $<

%.dvi: %.tex
	latex $<

clean: 
	rm -f ${RESULT}
