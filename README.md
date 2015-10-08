# 135_Project3

Final project for CSCI 135. 

/********************************************************************************************************************************************************
 Title: Uribe_hw3.cpp
 Author: Jose Antonio Uribe
 Created: May 12, 2013
 Purpose: The program has two options, DNA Heuristics and RNA Amino Acid Converter
 DNA Heuristics will look through a files DNA stands line per line, and do the following assuming the strand is ok:
    Find the complement of the strand, and save in 3' 5' instead of 5' 3'.
    Find the locations of all PolyT strands of 4 or greater, and CpG island locations
    It will also find the percentage of C's and G's.
RNA Conversion will translate an RNA sequence into it's corresponding Amino Acids.
It will take into account acceptable start and end codons, ignore RNA's until another start codon is found, and take in only
acceptable parameters of a string divisible by 3 and has only correct letters.

 Build with: g++ -o hw3 codon.cpp Uribe_hw3.cpp
 Modifications:
********************************************************************************************************************************************************/

