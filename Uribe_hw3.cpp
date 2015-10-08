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

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "codon.h"
using namespace std;

//DNA HEURISTICS FUNCTIONS
void fileCheck(ifstream& file);//Will check if the file can open or not. Will close if it cannot be open.
void fillVector(vector<string>& v, ifstream& file);//Will fill a vector of strings with each line into its elements
double CGCount(vector<string>& v, int iteration);//Will return the percentage of C's and G's
string strandComplement(vector<string>& v, int iteration);//Will return the complement of the string in 3' 5' form.
void CpGIslandCounter(vector<string>& v, int iteration, ofstream& file);//Will return all locations of CpG islands, if there are.
void polyT(vector<string>& v, int iteration, ofstream& file);//Will return the location of all PolyT strands, if there are any.
//RNA CONVERSTION FUNCTIONS
void convertToNum(string codon, Codon& one);//Will convert a codon into it's numeric equivalent.
void convertToAminoAcid(char a[4][4][4], Codon& one, string codon, string& aacids);//Using a codon's numeric equivalent,
                                                                                   //the codon will be translated into an amino acid.
void verifyCorrectness(string& RNA);//Will verify the correctness of the RNA string input.

void DNAHeuristics();//Will call the function that handles DNA heuristics.
void RNAConverter(char a[4][4][4]);//Will call the function that handles RNA conversion.

void newLine();

int main()
{
    //All necessary variables needed for DNA Heuristics and RNA conversion are called here.
    string RNA, codon, aacids, startcheck, endcheck, fileName, complement, result;
    Codon one;
    const string textOne = "Strand: ", textTwo = "Complement of: ",
    textThree = "GC Percentage: ", textFour = "CpG Islands: ", textFive = "PolyT Strands: ";
    one.stopstart = true;
    char query, database[4][4][4] = {{{'F','F','L','L'}, {'S','S','S','S'}, {'Y','Y','X','X'}, {'C','C','X','W'}},
                                    {{'L','L','L','L'}, {'P','P','P','P'}, {'H','H','Q','Q'}, {'R','R','R','R'}},
                                    {{'I','I','I','M'}, {'T','T','T','T'}, {'N','N','K','K'}, {'S','S','R','R'}},
                                    {{'V','V','V','V'}, {'A','A','A','A'}, {'D','D','E','E'}, {'G','G','G','G'}}};
    stringstream ss;
    ofstream newfiles;
    vector<string> strands;

    do
    {
        //So long as the user puts in a value for 1 or 2, the function will cycle through this menu.
        cout << "Welcome to the DNA/RNA analyzer program!" << endl;
        cout << "1) DNA Heuristics" << endl;
        cout << "2) RNA Amino Acid Converter" << endl;
        cout << "q) End program" << endl;
        cout << "enter an option from the menu above..." << endl;
        cin >> query;
        newLine();
        cout << endl;

        switch(query)
        {
        case '1':
            DNAHeuristics();//Calls DNAHeuristics: will go through analyzation of a file's DNA strands
        break;
        case '2':
            RNAConverter(database);//Using the database, an RNA converter given inside RNAConverter will convert RNAs.
        break;
        default:
        break;

        }
    } while (query != 'q' && (query == '1' || query == '2'));

    return 0;
}

void DNAHeuristics()
//Using it's related functions, will analyze a file of DNA strands line by line.
{
    string fileName, complement, result;

    const string textOne = "Strand: ", textTwo = "Complement of: ",
    textThree = "GC Percentage: ", textFour = "CpG Islands: ", textFive = "PolyT Strands: ";

    double percent;

    int num = 1;

    stringstream ss;
    ofstream newfiles;

    cout << "Enter the name of your file; remember to put .txt at the end!" << endl;
    cin >> fileName;
    cout << endl;

    ifstream DNA(fileName.c_str());//The file name be used to open the file here, through conversion to a c string
    fileCheck(DNA);

    vector<string> strands;

    fillVector(strands, DNA);

    for(int i = 0; i < int(strands.size()); i++)
    {
        percent = CGCount(strands, i);//Percent is saved here.
        complement = strandComplement(strands, i);
        if(complement == "error")//If complement returns error, this strand will be ignored; it's invalid.
        {
            num++;
            continue;
        }

        ss << num;//A quick conversion of an integer value to a string is done here. It is to create files of numeric order.
        result = ss.str() + ".txt";
        newfiles.open(result.c_str());


        cout << textOne << i+1 << endl;
        cout << strands[i] << endl;//The ith strand is printed here.
        cout << "__________________________" << endl;
        cout << textTwo << i+1 << endl;
        cout << complement << endl;//Its complement is printed here.
        cout << "__________________________" << endl;
        newfiles << textTwo << i+1 << endl;//And is saved to the file here.
        newfiles << complement << endl;
        cout << textThree <<  percent << "%" << endl;
        cout << "__________________________" << endl;
        newfiles << textThree << percent << "%" << endl;//Its CG percentage is saved and printed here.

        CpGIslandCounter(strands, i, newfiles);//Calling CpGIsland Counter, all CpG islands are found.
        cout << "__________________________" << endl;

        polyT(strands, i, newfiles);//Calling polyT, all polyT strands are located.
        cout << "==========================" << endl;


            num++;
            newfiles.close();
            ss.clear();//clears the sstream to avoid weird naming of files.
            ss.str("");
    }
}

void fillVector(vector<string>& v, ifstream& file)
//While the file has something to write to the vector, the vector will be filled with however many lines there are
//in the file.
{

    string thisLine;
    while(file >> thisLine)
    {
        v.push_back(thisLine);
    }
}

double CGCount(vector<string>& v, int iteration)
//Will calculate the percentage of C's and G's in a DNA strand
{
    string thisStrand = v[iteration];
	double total = 0, Length = thisStrand.length(), percentage = 0;

	for(int i = 0; i < Length; i++)
	{
        if(thisStrand[i] == 'C' || thisStrand[i] == 'G')
            total++;
	}

    percentage = ((double(total)) / double(Length)) * 100;

	return percentage;
}

string strandComplement(vector<string>& v, int iteration)
//This function does two things: Creates a complement of the argument string, and verifies if all letters are acceptable
{
    string thisStrand = v[iteration], complement;

    for(int i = thisStrand.length() - 1; i >= 0; i--)//If the character at the ith element is valid, it's complemet will be returned
    {
        if(thisStrand[i] == 'A')
            complement += "T";
        else if (thisStrand[i] == 'T')
            complement += "A";
        else if(thisStrand[i] == 'C')
            complement += "G";
        else if(thisStrand[i] == 'G')
            complement += "C";

        //Otherwise, the function will reject the strand and return an string of "error"
        if((thisStrand[i] != 'A' && thisStrand[i] != 'T' && thisStrand[i] != 'C' && thisStrand[i] != 'G'))
        {
            cout << "Oh! It seems DNA strand " << iteration+1 << " contains invalid characters." << endl;
            cout << "Please make sure that all characters are A,C,T, and G" <<endl;
            cout << "==========================================================================" << endl;
            return "error";
        }
    }

    return complement;
}

void CpGIslandCounter(vector<string>& v, int iteration, ofstream& file)
//CpG Island Locator will return all locations of CpG islands; C's directly behind G's.
{
    int counter = 0;
    string thisStrand = v[iteration];

    cout << "CpG Island Locations: " << endl;
    file << "CpG Island Locations: " << endl;
    for(int i = 1; i < int(thisStrand.length()); i++)
    {

        if(thisStrand[i-1] == 'C' && thisStrand[i] == 'G')//Only if the ith element is G, and the i-1 element is C will this run
        {
            counter++;
            cout << "Location " << counter << ": " << i << "-" << i+1 << endl;//Presents the location of the CpG island
            file << "Location " << counter << ": " << i << "-" << i+1 << endl;//And then writes it to the file passed into the func.

        }
    }
    if(counter == 0)//Otherwise no CpG islands were found and that fact will be written to the file
    {
        cout << "No CpG islands were found for this file." << endl;
        file << "No CpG islands were found for this file." << endl;
    }
}

//Locates all PolyT strands of 4 or more.
void polyT(vector<string>& v, int iteration, ofstream& file)
{
    string thisStrand = v[iteration];
    int polyT = 0, firstT = 0;

    cout << "Locations of PolyT strands of 4 or greater:" << endl;
    file << "Locations of PolyT strands of 4 or greater:" << endl;
    for(int i = 3; i < int(thisStrand.length()); i++)
    {
        //If all 4 preceding values of a strand are T, a polyT strand has been found.
        if(thisStrand[i] == 'T' && thisStrand[i-1] == 'T' && thisStrand[i-2] == 'T' && thisStrand[i-3] == 'T')
        {
            polyT++;
            firstT = i-2;//The location of the first value if starting from 1 will simply be i-2.
            while(thisStrand[i] == thisStrand[i+1])
            {
                i++;//So long as the next element after the last T is T, these elements will be skipped by incrementing i.
            }
            cout << "Location of strand " << polyT << ": " << firstT << "-" << i+1 << endl;
            file << "Location of strand " << polyT << ": " << firstT << "-" << i+1 << endl;//The location of all polyT strands are
                                                                                        //Written to the file and printed here.

        }
    }

    if(polyT == 0)//If no polyT strands were located, it is saved to the file.
    {
        cout << "No polyT strands were found for this strand of DNA." << endl;
        file << "No polyT strands were found for this strand of DNA." << endl;
    }
}

void RNAConverter(char a[4][4][4])
//Using the databse, RNAConverter converts codons to their corresponding element in the 3D array
{

   string RNA, codon, aacids, startcheck, endcheck;
   Codon one;

   cout << "Please enter a string of RNA; be sure to start and end with appropriate codons!" << endl;
   cin >> RNA;//A string of RNA is input here by the user.
   cout << endl;
   verifyCorrectness(RNA);//Verifies the acceptability of the RNA string. Will continue asking for input until something is done.

   for(int i = 0; i < int(RNA.length()); i+=3)//Done in increments of 3, the string is analyzed a codon at a time.
   {
       codon = RNA.substr(i, 3);
       convertToNum(codon, one);

       if(one.whole == 22 || one.whole == 23 || one.whole == 32)//This if/else statement checks if the codons at this point are
                                                                //either a stop or start codon.
       {
             one.stopstart = false;//If it's a stop codon, the codon is set to false.
       }
       else if (one.whole == 203)
       {
             one.stopstart = true;//if it's a start codon, the codon is set to true.

       }
       convertToAminoAcid(a, one, codon, aacids);//Converts the codon into an amino acid
       codon = "";
   }


    cout << "Converted RNA sequence's corresponding Amino Acids: " << endl;
    cout << aacids << endl;//Prints the amino acid character string.
    cout << endl;
}

void convertToNum(string codon, Codon& one)
//Converts the codons into their numeric counterparts. Stores each RNA character in a member value of struct codon.
{

        if(codon[0] == 'U')
            one.first = 0;
        else if(codon[0] == 'C')
            one.first = 1;
        else if(codon[0] == 'A')
            one.first = 2;
        else if(codon[0] == 'G')
            one.first = 3;

        if(codon[1] == 'U')
            one.second = 0;
        else if(codon[1] == 'C')
            one.second = 1;
        else if(codon[1] == 'A')
            one.second = 2;
        else if(codon[1] == 'G')
            one.second = 3;

        if(codon[2] == 'U')
            one.third = 0;
        else if(codon[2] == 'C')
            one.third = 1;
        else if(codon[2] == 'A')
            one.third = 2;
        else if(codon[2] == 'G')
            one.third = 3;

        one.whole = (one.first * 100) + (one.second * 10) + one.third;//Converts the amino acid into a whole number
}

void convertToAminoAcid(char a[4][4][4], Codon& one, string codon, string& aacids)
{
    if(one.stopstart)//If the codon is after a start codon, this will run. otherwise, it is after an end codon. it will be ignored if so.
    {
    for(int i = 0; i < 4; i++)
    {
        if( i == one.first)//Will search for the 1st dimension of its corresponding database equivalent.
        {
            for(int j = 0; j < 4; j++)
            {
                if(j == one.second)//Will search for the 2nd dimension of its corresponding database equivalent
                {
                    for(int k = 0; k < 4; k++)
                    {
                        if (k == one.third)//Will search for the 3rd dimension of its corresponding database equivalent.
                        {
                            aacids += a[i][j][k];//If all 3 have been found, its amino acid will be appended to the string aacids
                        }
                    }
                }
            }
        }

    }
    }
}

void verifyCorrectness(string& RNA)
{
     bool correct = false;
     Codon checkfirst, checklast;
     string first, last;

     while(!(correct))
     {

         for(int i = 0; i < int(RNA.length()); i++)
         {
             if( !(RNA[i] == 'U' || RNA[i] == 'C' || RNA[i] == 'A' || RNA[i] == 'G') || !(RNA.length() % 3 == 0))
            //If the string consists of anything but U, C, A, G, or isn't divisible by 3, the string is rejected a new one is given
             {
                 cout << "Incorrect parameters; enter a new string" << endl;
                 cin >> RNA;
                 break;
             }
            first = RNA.substr(0, 3);//Saves the first codon.
            convertToNum(first, checkfirst);
            last = RNA.substr(RNA.length() - 3, 3);//And the last codon.
            convertToNum(last, checklast);
             if(!((checkfirst.whole == 203) && (checklast.whole == 22 || checklast.whole == 23 || checklast.whole == 32)))
             {//If the first and second codons aren't start and end codons, the line is rejected, and the loop will go around and
                 //check if the string is an acceptable length and isn't anything but U's, C's, A's and G's
                 cout << "Incorrect parameters; enter a correct start and end codon" << endl;
                 cin >> RNA;
                 break;
             }
             if(i+1 == int(RNA.length()))//If the loop makes it to the end, the RNA is valid.
                correct = true;
         }
     }
}

void newLine()
//newLine makes sure any unecessary characters are not read.
//EX: if the program asks for n, and the user enters "name name name"
//It will only read n and discard the rest.
{
    char symbol;

    do
    {
        cin.get(symbol);
    } while( symbol != '\n');
}

void fileCheck(ifstream& file)
//simply put, this function will check if the file is within the same directory as the executable.
//Otherwise, the program will end, prompting the changing of location of the file.
{
    if(file.fail())
    {
        cerr << "Input file could not be opened. Check if the file is in the same directory" << endl
             << " as the executable.\n";
        return;
    }
}

