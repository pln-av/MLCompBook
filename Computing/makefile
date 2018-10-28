lstsetup = ${BOOK_DIR}/util/listings-setup.tex
optsetup = ${BOOK_DIR}/util/options-setup.sty
inputs = cl1.md cl2.md cl3.md clmkl.md
computing.pdf : $(inputs)
	pandoc -s -o computing.pdf $(inputs) --listings -H $(lstsetup) -H $(optsetup)

clean : 
	rm computing.pdf