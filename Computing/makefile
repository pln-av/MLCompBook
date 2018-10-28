lstsetup = ../../listings-setup.tex
optsetup = ../../options-setup.sty
inputs = cl1.md cl2.md cl3.md clmkl.md
computing.pdf : $(inputs)
	pandoc -s -o computing.pdf $(inputs) --listings -H $(lstsetup) -H $(optsetup)

clean : 
	rm computing.pdf