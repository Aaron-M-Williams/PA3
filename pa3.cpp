#include "pa3.h"
#include <fstream>

using namespace std;

string keywordsUsed = "";
string identifiers = "";
string constants = "";
string operatorsUsed = " ";
string delimitersUsed = "";
string errorsUsed = "";



char nextCharacter(string line, int* ptrPosition);
string lineRemaining(string line, int* ptrPosition);
void addIdentifier(string line, int* ptrPosition);
void addOperator(string line, int* ptrPosition);
void addConstant(string line, int* ptrPosition);



int main()
{
    //Stack using linkedList type of data structure, to hold all keywords used.
    Stack keyStack;
    //Stack using linkedList style to hold all parenthesis used.
    Stack StackParen;

    string line;
    int position = 0;
    int* ptrPosition = &position;


    ifstream in;
    string filePath = "";
    cout << "Please enter the name of the input file: ";
    cin >> filePath;

    in.open(filePath.c_str());



    if (!in.is_open())
    {
        cerr << "File not found. Terminating Program..." << endl;
        return 0;
    }




    while (getline(in, line))
    {

        std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
        line.erase(end_pos, line.end());


        position = 0;

        if (line[0] == 'F' && nextCharacter(line, ptrPosition) == 'O' && nextCharacter(line, ptrPosition) == 'R')
        {

            if (nextCharacter(line, ptrPosition) == '(')
            {

                if (keywordsUsed.find("FOR") == string::npos)
                {
                    keywordsUsed += "FOR ";
                }


                StackParen.insert("(");


                while (nextCharacter(line, ptrPosition) == '(')
                {
                    StackParen.insert("(");
                }

                *ptrPosition -= 1;
            }


            else
            {
                *ptrPosition -= 3;
                errorsUsed += line[0] + lineRemaining(line, ptrPosition) + " ";
                continue;
            }

           //first part of for loop is variable
            addIdentifier(line, ptrPosition);
            *ptrPosition += 1;

            //second entry in loop is constant
            addConstant(line, ptrPosition);
            *ptrPosition += 1;

            //third entry in loop is operator
            addOperator(line, ptrPosition);

            int closeParenCount = 0;
            while (nextCharacter(line, ptrPosition) == ')')
            {
                StackParen.insert(")");
                closeParenCount += 1;
            }
            *ptrPosition = *ptrPosition- closeParenCount;

            //If anything else on line, put it in error.

            if (lineRemaining(line, ptrPosition) != "")
            {
                errorsUsed += lineRemaining(line, ptrPosition) + " ";
            }

            //insert for if it is valid
            else
            {
                keyStack.insert("FOR");
            }


            if (delimitersUsed.find(",") == string::npos)
            {
                delimitersUsed += ", ";
            }
        }

            //if line starts with "BEGIN"
        else if (line[0] == 'B' && nextCharacter(line, ptrPosition) == 'E' && nextCharacter(line, ptrPosition) == 'G' && nextCharacter(line, ptrPosition) == 'I' && nextCharacter(line, ptrPosition) == 'N')
        {
            //If there is anything after begin, add it to error.
            if (nextCharacter(line, ptrPosition) > 31 && nextCharacter(line, ptrPosition) < 127)
            {
                *ptrPosition -= 2;
                errorsUsed += lineRemaining(line, ptrPosition) + " ";
            }
            //Add begin to the stack if correct.
            else
            {
                
                if (keywordsUsed.find("BEGIN") == string::npos)
                {
                    
                    keywordsUsed += "BEGIN ";
                    
                }

                keyStack.insert("BEGIN");
            }
        }
        //END
        else if (line[0] == 'E' && nextCharacter(line, ptrPosition) == 'N' && nextCharacter(line, ptrPosition) == 'D')
        {

            //If anything after End, add it to error.
            if (nextCharacter(line, ptrPosition) > 31 && nextCharacter(line, ptrPosition) < 127)
            {
                
                *ptrPosition = 0;
                errorsUsed += line[0] + lineRemaining(line, ptrPosition) + " ";
                
            }


            //If valid end, add it to stack.
            else
            {
                
                if (keywordsUsed.find("END") == string::npos)
                {
                   
                    keywordsUsed= keywordsUsed+ "END ";
                    
                }

                keyStack.insert("END");
            }
        }
            //Incoming statement with variables.
        else if (keyStack.top() == "FOR"|| keyStack.top() == "BEGIN")
        {
            
            //Add lower to error list
            
            if (!islower(line[0]))
            {
                errorsUsed += line;
                continue;
            }



            addIdentifier(line, ptrPosition);
            addOperator(line, ptrPosition);

            while (nextCharacter(line, ptrPosition) != ';')
            {
                *ptrPosition -= 1;




                //Assuming that a lower case letter means variable
                if (islower(nextCharacter(line, ptrPosition)))
                {
                   
                    *ptrPosition -= 1;
                    addIdentifier(line, ptrPosition);
               
                }
                    //Assume digit is a constant
                
                
                else if (isdigit(nextCharacter(line, ptrPosition)))
                {
                    
                    *ptrPosition -= 2;
                    addConstant(line, ptrPosition);
                
                }
                    //Else assume that it is an operator
                
                
                else
                {
                    *ptrPosition -= 2;
                    addOperator(line, ptrPosition);
                
                }
            }




            if (delimitersUsed.find(";") == string::npos)
            {
                
                delimitersUsed = delimitersUsed+ "; ";
            }
        }

        else
        {
            
            errorsUsed = errorsUsed+ line[0] + lineRemaining(line, ptrPosition);
        
        }
    }

    in.close();

    cout << "\nThe depth of nested loop(s) is " << keyStack.depthOfLoops() << endl;

    cout << "Keyword(s) found: " << keywordsUsed << endl;

    cout << "Identifier(s) found: " << identifiers << endl;

    cout << "Constant(s) found: " << constants << endl;

    cout << "Operator(s) found:" << operatorsUsed << endl;

    cout << "Delimiter(s) found: " << delimitersUsed << endl;

    if(errorsUsed !=""){
        cout << "\nSyntax Error(s) found: " << errorsUsed;
    }
    else{
        cout<<"\nSyntax Error(s) found: N/A";
    }

    return 0;
}


void addConstant(string line, int* ptrPosition)
{

    string output = "";


    while (isdigit(nextCharacter(line, ptrPosition)))
    {
        *ptrPosition -= 1;
        output += nextCharacter(line, ptrPosition);
    }


    *ptrPosition -= 1;

    //if it's not already a listed constant, add it
    if (constants.find(constants) == string::npos)
    {
        constants += output + " ";
    }
}


//Add Indentifier
void addIdentifier(string line, int* ptrPosition)
{
    string variable = "";

    if (*ptrPosition == 0)
    {
        variable += line[0];
    }




    //combine lower case letters
    while (islower(nextCharacter(line, ptrPosition)))
    {
        *ptrPosition -= 1;
        variable += nextCharacter(line, ptrPosition);
    }


    *ptrPosition -= 1;



    if (identifiers.find(variable + " ") == string::npos)
    {
        identifiers += variable + " ";
    }



    return;
}


string lineRemaining(string line, int* ptrPosition)
{
    //Number of characters
    int count = 0;
    string result = "";



    while (nextCharacter(line, ptrPosition) > 31)
    {
        *ptrPosition -= 1;
        if (nextCharacter(line, ptrPosition) < 127)
        {
            *ptrPosition -= 1;
            result += nextCharacter(line, ptrPosition);
        }
        else
        {
            break;
        }
        count += 1;
    }

    //Goes back to position
    *ptrPosition -= (count+1);
    return result;
}

//addOperator
void addOperator(string line, int* ptrPosition)
{
    if (nextCharacter(line, ptrPosition) == '+')
    {
        if (nextCharacter(line, ptrPosition) == '+')
        {
            if (operatorsUsed.find("++ ") == string::npos)
            {
                operatorsUsed += "++ ";
            }
            return;
        }

        *ptrPosition -= 1;

        if (operatorsUsed.find(" + ") == string::npos)
        {
            operatorsUsed += "+ ";
        }

        return;
    }

    *ptrPosition -= 1;
    if (nextCharacter(line, ptrPosition) == '-')
    {
        if (operatorsUsed.find("-") == string::npos)
        {
            operatorsUsed += "- ";
        }

        return;
    }

    *ptrPosition -= 1;
    if (nextCharacter(line, ptrPosition) == '/')
    {
        if (operatorsUsed.find("/") == string::npos)
        {
            operatorsUsed += "/ ";
        }

        return;
    }

    *ptrPosition -= 1;
    if (nextCharacter(line, ptrPosition) == '*')
    {
        if (operatorsUsed.find("*") == string::npos)
        {
            operatorsUsed += "* ";
        }

        return;
    }

    *ptrPosition -= 1;
    if (nextCharacter(line, ptrPosition) == '=')
    {
        if (operatorsUsed.find("=") == string::npos)
        {
            operatorsUsed += "= ";
        }
    }

    return;
}


char nextCharacter(string line, int* ptrPosition)
{
    *ptrPosition =*ptrPosition+1;
    int next = *ptrPosition;
    char nextCharacter=line[next];
    return nextCharacter;
}


template<typename T, typename P>
T remove_if(T beg, T end, P pred)
{
    T dest = beg;
    for (T itr = beg;itr != end; ++itr)
        if (!pred(*itr))
            *(dest++) = *itr;
    return dest;
}