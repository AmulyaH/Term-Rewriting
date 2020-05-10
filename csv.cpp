#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

using CSV = vector<vector<string>>;

CSV parseCSV(istream& in);

template<typename IN, typename PRED, typename F>
void split(IN begin, IN end, PRED p, F fill)
{
    IN current = begin;
    IN next = begin;
    while(next != end)
    {
        next = find_if(current, end, p);
        fill(current,next);
        if(next != end)
        {
            next++;
            current = next;
        }
    }
}

int main(int argc,char** argv)
{
    char* filename = argv[1];
    ifstream in(filename);
    CSV csv = parseCSV(in);

    for(vector<string> row : csv)
    {
        for(string& col : row)
        {
            cout << col << "|";
        }

        cout << endl;
    }
}

CSV parseCSV(istream& in)
{
    CSV csv;

    string text(istreambuf_iterator<char> {in},
                istreambuf_iterator<char>());

    split(text.begin(), text.end(), 
        [](char c){return c = '\n';},
        [&csv](auto line_begin, auto line_end)
        {
            vector<string> row;
            bool quoted = false;
            split(line_begin, line_end,
            [&quoted](char c)
            {
                if(c == '\"') quoted = !quoted;
                return c == ',' && !quoted;
            },
            [&row](auto cell_begin, auto cell_end)
            {
                row.push_back(string(cell_begin, cell_end));
            });

            csv.push_back(row);
        });
        
    return csv;
}
