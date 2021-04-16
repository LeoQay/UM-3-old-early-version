#include "Processor.h"
#include "Exceptions.h"
#include "SFML/Window.hpp"

#include <fstream>

int main()
{
    ifstream fin("punched_card.txt");
    ofstream fout("result.txt");

    try
    {
        Processor proc_obj;

        proc_obj.input_punched_card(fin);

        proc_obj.main_process();

        proc_obj.output_memory(fout);

        proc_obj.clear_memory();
    }

    catch (Out_range& err)
    {
        cout << err.message << " " << err.bad_index;
        return 1;
    }

    catch (Bad_command& err)
    {
        cout << err.message << " " << err.what_is;
        return 2;
    }

    catch (Empty& err)
    {
        cout << err.message;
        return 3;
    }

    fin.close();
    fout.close();

    return 0;
}