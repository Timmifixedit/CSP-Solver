Source :=	report.tex
PDF :=		$(patsubst %.tex,%.pdf,$(Source))
BiberFile :=	$(patsubst %.tex,%.bcf,$(Source))
Class :=	cpp-hmwk.cls
Temporaries :=	$(patsubst %.tex,%.aux,$(Source)) \
		$(patsubst %.tex,%.log,$(Source)) \
		$(patsubst %.tex,%.out,$(Source)) \
		$(patsubst %.tex,%.bbl,$(Source)) \
		$(patsubst %.tex,%.bcf,$(Source)) \
		$(patsubst %.tex,%.blg,$(Source)) \
		$(patsubst %.tex,%.run.xml,$(Source))

$(PDF):		$(Source) $(Class)
		lualatex $(Source)
		biber $(BiberFile)
		lualatex $(Source)

.PHONY:		clean realclean
clean:
		rm -f $(Temporaries)
realclean:	clean
		rm -f $(PDF)
