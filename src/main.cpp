#pragma once
#include "Processor.h"
#include "Exception.h"

#include <fstream>

int main()
{
    ifstream fin("punched_card.txt");
    ofstream fout("result.txt");

    Processor processor;

    try {
        processor.Input_PunchedCard(fin);

        processor.main_process();

        processor.outMemory(fout);
    }

    catch (Exception& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        return 1;
    }

    fin.close();
    fout.close();

    return 0;
}
