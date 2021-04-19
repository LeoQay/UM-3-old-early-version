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
    }

    catch (IndexOutRange& err)
    {
        cout << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    catch (Bad_token& err)
    {
        cout << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    catch (Empty& err)
    {
        cout << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    try{
        proc_obj.main_process();
    }

    catch (Bad_command& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    catch (NULL_DIVIDE& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    catch (FTOIOutRange& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    catch (MathOutRange& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        cout << proc_obj.output_stat();
        return 1;
    }

    proc_obj.output_memory(fout);

    proc_obj.clear_memory();

    fin.close();
    fout.close();

    return 0;
}