optsetup = ${BOOK_DIR}/util/options-setup.sty
mathsetup = ${BOOK_DIR}/util/math-setup.tex
inputs = la-ops.md
la.pdf : $(inputs) $(mathsetup)
	pandoc -s -o la.pdf $(inputs) --listings -H $(optsetup) -H $(mathsetup)

la-test.pdf : la-test.md $(mathsetup)
	pandoc -s -o la-test.pdf la-test.md -H $(optsetup) -H $(mathsetup)

clean-test : 
	rm la-test.pdf

clean :
	rm la-test.pdf la.pdf