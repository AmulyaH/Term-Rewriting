#include<iostream>
#include<string>
#include<fstream>

using namespace std;

string** parseCSV(istream& in, int& rows, int& cols);

int main(int argc,char** argv)
{
    char* filename = argv[1];
    int rows = 0;
    int cols = 0;
    ifstream in(filename);
    string** csv = parseCSV(in, rows, cols);

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j< cols; j++)
        {
            cout << csv[i][j] << ",";
        }

        cout << endl;
    }
}

string** parseCSV(istream& in, int& rows, int& cols)
{
    string line;

    while(getline(in,line))
    {
        rows++;
        int rowCols = 0;
        bool quote = false;
        for(int i=0; i<line.size(); i++)
        {
            if(line[i] == '\"')
            {
                quote = !quote;
            }
            if(line[i] == '|')
            {
                rowCols++;
            }
        }
        rowCols++;
        cols = max(cols, rowCols);
    }

    string** csv = new string*[rows];
    for(int i=0; i< rows; i++)
    {
        csv[i] = new string[cols];
    }

    in.seekg(0,in.beg);
    in.clear();

    int r = 0;
    while(getline(in,line))
    {
        bool quote = false;
        int c = 0;
        for(int i=0; i<line.size(); i++)
        {
            if(line[i] == '\"')
            {
                quote = !quote;
            }
            if(line[i] == ',' && !quote)
            {
                c++;
            }
            else
            {
                csv[r][c] += line[i];
            }
        }
        r++;
    }
    return csv;
}
