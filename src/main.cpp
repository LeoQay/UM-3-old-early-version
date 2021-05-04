#pragma once
#include "Processor.h"
#include "Exception.h"

#include <fstream>

int main()
{
    ifstream fin("punched_card.txt");
    ofstream fout("result.txt");
    ofstream logFile("log.txt");

    auto processor = new Processor(logFile);

    try
    {
        (*processor).Input_PunchedCard(fin);
    }

    catch (Exception& err)
    {
        logFile << "Punched card load fail\n";
        cout << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";
        logFile << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";

        delete processor;
        cin.get();
        fin.close();
        fout.close();
        logFile.close();
        return 1;
    }

    try{
        (*processor).main_process();
    }

    catch (Exception& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        logFile << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";

        delete processor;
        cin.get();
        fin.close();
        fout.close();
        logFile.close();
        return 1;
    }

    fout << (*processor).outMemory();

    delete processor;
    cin.get();
    fin.close();
    fout.close();
    logFile.close();
    return 0;
}