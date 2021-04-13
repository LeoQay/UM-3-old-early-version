#include <fstream>
#include "Processor.h"

int main()
{
    Processor proc_obj;

    ifstream fin("punched_card.txt");
    ofstream fout("result.txt");

    proc_obj.input_punched_card(fin);

    proc_obj.main_process();

    proc_obj.output_memory(fout);

    proc_obj.clear_memory();

    fin.close();
    fout.close();

    return 0;
}