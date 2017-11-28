//
//  main.cpp
//  SOPFinal
//
//  Created by Clarissa Miranda on 27/11/17.
//  Copyright © 2017 Clarissa Miranda. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

int swa[4096]={0};
int mp[127]={0};
int m[2048]={0};
int iC=0;
int iLibre=128;
vector<pair<int,int>> fifoP;

void liberaProceso(int iP)
{
    //Elimina proceso del arreglo
    int iTam = fifoP.size();
    cout<< "iTam " << iTam << endl;
    for(int i=0;i<127;i++)
    {
        if(mp[i]==iP)
        {
            mp[i]=0;
        }
    }
    /*
    //Elimina proceso del vector
    for(int i=0;i<iTam;i++)
    {
        if(fifoP[i].first==iP)
        {
            cout << "first: " << fifoP[i].first << endl;
            //fifoP.erase(fifoP.begin() + i-1);
            //iLibre++;
        }
    }*/
}


void imprimirArreglo()
{
    for(int i=0;i<128;i++)
    {
        cout << mp[i] << endl;
    }
}

void swapFifo(int iN, int iP)
{
    int temp;
    iC = ceil(iN/16.0);
    for(int i=0;i<iC;i++)
    {
        if((swa[i]==0)&&iC>0)
        {
            temp = fifoP[0].first;
            swa[i]=temp;
            iC--;
        }
    }
    iLibre -= iC;
}

// Función que coloca un proceso en memoria ejecutable
void colocarProceso(int iN, int iP)
{
    iC = ceil(iN/16.0);
    if(iLibre >= iC)
    {
        iLibre -= iC;
        for(int i=0;i<128;i++)
        {
            if(mp[i]==0&&iC>0)
            {
                mp[i]=iP;
                fifoP.push_back(make_pair(iP,i));
                iC--;
            }
        }
    }
    else
    {
        // swapFifo(iN,iP);
    }
}

void iniciarArchivo()
{
    ifstream archivoLeer;
    char cOpc;
    int iD,iN,iP,iM;
    string sReplica;
    
    archivoLeer.open("prueba.txt");
    
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
            colocarProceso(iN, iP);
            
        }
        if(cOpc=='A')
        {
            archivoLeer >> iD >> iP >> iM;
        }
        if(cOpc=='L')
        {
            archivoLeer >> iP;
            liberaProceso(iP);
        }
        if(cOpc=='F')
        {
            
        }
        if(cOpc=='E')
        {
            imprimirArreglo();
        }
    }
    
}

int main() {

    iniciarArchivo();
    
    return 0;
}
