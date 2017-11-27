//
//  main.cpp
//  SOPFinal
//
//  Created by Clarissa Miranda on 27/11/17.
//  Copyright © 2017 Clarissa Miranda. All rights reserved.
//

// aquí Flor dejando huella pa ver si funciona el PUCH

#include <iostream>
#include <fstream>

using namespace std;

void iniciarArchivo(ifstream archivoLeer,ostream output)
{
    char cOpc;
    int iD,iN,iP,iM;
    string sReplica;
    
    archivoLeer.open("VirtualMemoryTestFileFifoLru.txt");
    while(archivoLeer>>cOpc)
    {
        if(cOpc=='C')
        {
            getline(archivoLeer,sReplica);
            cout << sReplica << endl;
        }
        if(cOpc=='P')
        {
            archivoLeer >> iN >> iP;
        }
        if(cOpc=='A')
        {
            archivoLeer >> iD >> iP >> iM;
        }
        if(cOpc=='L')
        {
            archivoLeer >> iP;
        }
        if(cOpc=='F')
        {
            output << endl;
        }
    }
    
}

int main() {
    
    ifstream archivoLeer;
    
    return 0;
}
