
TARGETS = pw13_rapport_soutenance_3.pdf pw13_plan_de_soutenance_3.pdf

all: ${TARGETS}

pw13_rapport_soutenance_3.pdf: patchwork13.jpg $(wildcard *.tex)

patchwork13.jpg: ../cahier_des_charges/patchwork13.jpg
	cp -f $< $@

.PHONY: pub
pub: ${TARGETS}
	scp ${TARGETS} ssh.sf.net:/home/groups/p/pa/patchwork13/htdocs/doc/

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

%.dvi: %.tex
	latex $<
	latex $<

clean: 
	rm -f ${TARGETS} *.toc patchwork13.jpg
