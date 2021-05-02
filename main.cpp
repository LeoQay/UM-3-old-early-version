#pragma once
#include "Processor.h"
#include "Exceptions.h"

#include <fstream>

int main()
{
    ifstream fin("punched_card.txt");
    ofstream fout("result.txt");

    Processor proc_obj;

    try {
        proc_obj.input_punched_card(fin);

        proc_obj.main_process();

        proc_obj.output_memory(fout);

        proc_obj.clear_memory();
    }

    catch (Exceptions& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        return 1;
    }

    fin.close();
    fout.close();

    return 0;
}