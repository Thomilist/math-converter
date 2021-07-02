Mathcad2LaTeX v1.4
af Thomas Emil Jensen med AutoHotkey 1.1.24.00



INSTRUKTION

Genveje:
- CTRL + SHIFT + V: Konvertér og indsæt fra Mathcad til LaTeX.
- Højre CTRL + Højre SHIFT + R: Genindlæs programmet.
- Højre CTRL + Højre SHIFT + E: Afslut programmet.

Normal anvendelse:
0. Programmet kan skrive LaTeX-kode, som kræver pakken Mathtools. Brug \usepackage{mathtools} i starten af dokumentet.
1. Kopiér elementer i matematikfelter i Mathcad (grå markering indeni feltet, ikke blå markering af hele feltet).
2. Tryk CTRL + SHIFT + V i din LaTeX-editor, og udtrykket indsættes som LaTeX-kode.

Demonstration:
https://gfycat.com/EachDisgustingKookaburra

Indstillinger:
- Indstillinger tilgås ved at "konvertere" (@SETUP INDSTILLING VÆRDI).
- INDSTILLING er indstillingstypen, mens VÆRDI er indstillingens værdi (0 eller 1).
- Mulige indstillinger: 
	- comma (komma eller punktum som decimaltalsseparator).
	- debug (print mellemtrin fra konverteringen)

Bemærkninger og kendte problemer:
- Programmet laver flere akkolader (også kaldet "tuborgklammer") end nødvendigt, men disse har ingen praktisk betydning.
- Programmet melder "Fejl 1", hvis det ikke kan genkende syntaksen (fx, hvis det kopierede emne ikke er fra Mathcad). Programmet gemmer en fejlrapport og genindlæses automatisk.
- Programmet melder "Fejl 2", hvis det støder på en ukendt operator. Programmet gemmer en fejlrapport og genindlæses automatisk.
- Hvis der går flere sekunder, uden der sker noget, er der sandsynligvis sket en fejl. Tryk Højre CTRL + Højre SHIFT + R for at genindlæse programmet.
- I nogle tilfælde kan konverteringen tilsyneladende foregå uden problemer, men ved kompilering i LaTeX returneres fejlen "! You can't use `\eqno' in math mode". Ofte kan dette løses ved at konvertere fra Mathcad til LaTeX igen. Årsagen er ukendt.
- Brøker i matricer har ikke altid helt nok plads. Mathcad2LaTeX tilføjer automatisk et linjeafstandsparater [0.0em], hvor 0.0 angiver ekstra linjeafstand. Denne værdi kan manuelt øges til en passende størrelse.
- Enheder og konstanter forsøges skrevet med normal tekst. Hvis disse alligevel skrives med kursiv, kan det være nødvendig manuelt at tildele "unit" eller "constant" labels i Mathcad og konvertere igen.
- Danske bogstaver, især å, kan forårsage fejl. Typisk er fejlen relateret til "accents in math mode" eller "\spacefactor in math mode"
- Imaginære tal formatteres ukorrekt med kursiv

Understøttede funktioner:
- Lighedstegn (definition, sammenligning og numerisk evaluering)
- Ulighedstegn (mindre end, større end, mindre end/lig med, større end/lig med, ikke lig med)
- Addition
- Subtraktion
- Multiplikation
- Division
- Potens
- Rod
- Parenteser
- Trigonometriske funktioner (cos, sin, tan osv.)
- Absolut værdi
- Symbolsk evaluering
- Funktioner
- Enheder
- Sænket skrift
- Græske bogstaver
- Uendelighedssymbol
- Matricer
- Differentiation
- Integraler
- Procent
- Gradtegn
- Faktorialer
- Logisk NOT, AND og OR
- Summation og produkt
- Krydsprodukt
- Polær notation
- Definition
- Element af
- Interval og stepinterval


ÆNDRINGER

v1.4:
- Tilføjet en genvejstast til afslutning af programmet.
- Indstillingen "debug" tilføjet.
- Gradtegn, faktorialer, logisk NOT/AND/OR, summation, produkt, krydsprodukt, polær notation, definition, element af, interval og stepinterval understøttes
- Logaritme, havelåge (#) og i nogle tilfælde gradtegn indsættes bedre
- De danske bogstaver æ, ø og å understøttes bedre, men ikke helt

v1.3:
- Indstillinger tilføjet.
- Konvertering af decimaltalsseparator fra punktum til komma understøttes.
- En fejl, hvor \;\mathrm{} ikke blev tilføjet til enheder, er rettet.

v1.2:
- Differentiation, integration og procent understøttes.
- Indsættelsen af tekst bør være hurtigere og mere robust - inputs, mens indsættelsen foregår, bør ikke have nogen indflydelse.

v1.1:
- Keywords inkluderes nu over pilen ved symbolsk evaluering.
- Matricer understøttes.

v1.0:
- Første udgivelse.