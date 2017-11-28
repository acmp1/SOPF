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
#include <unordered_map>

using namespace std;

int swa[4096]={0};
int mp[127]={0};
int m[2048]={0};
int iC=0;
int iLibre=128;
int iTimeStamp = 0;
int iSwaps = 0;
int iPageFaults = 0;

unordered_map<int,int> fifoP;

void imprimirMapa()
{
    for(auto it = fifoP.begin(); it != fifoP.end(); ++it)
    {
        cout << " " << it->first << ":" << it->second << endl;
    }
}

int mostrarDireccionReal(int iP, int iD)
{
    /*int aux=0;
    aux = iD/16;
    auto it = fifoP.end();
    for(; it != fifoP.begin() && aux>0; --it)
    {
        if(iP == it->second)
        {
            aux--;
        }
    }
    cout << "ALGO " << it->first*16 + iD%16 << endl;*/
    int iContR = 0;
    int iContP = 0;
    for(int i = 0; i < 128; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            if(iContP == iD)
            {
                return iContR;
            }
            iContR++;
            if(mp[i] == iP)
            {
                iContP++;
            }
        }
    }
    return 0;
}

void liberaProceso(int iP)
{
    
    int temp, mayor;
    int iC = 0;
    int iC2 = 0;
    int j;
    for(j=0; j<128; j++)
    {
        if(mp[j] == iP)
        {
            if(iC==0)
            {
                temp = j;
                iC++;
            }
            if(mp[j+1] != iP)
            {
                if(iC2==0)
                {
                    mayor = j;
                    iC2++;
                }
            }
        }
    }
    
    cout << "Se liberan los marcos de página de memoria real: " << temp << " - " << mayor << endl;
    //Elimina proceso del arreglo
    for(int i=0;i<127;i++)
    {
        if(mp[i]==iP)
        {
            iLibre ++;
            mp[i]=0;
            fifoP.erase(i);
        }
    }
    

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
            temp = fifoP.begin()->second;
            swa[i]=temp;
            iC--;
            iSwaps++;
        }
    }
    iLibre -= iC;
}

// Función que coloca un proceso en memoria ejecutable
void colocarProceso(int iN, int iP)
{
    iC = ceil(iN/16.0);
    iTimeStamp += iC;
    int n = iN;

    if(iLibre >= iC)
    {
        iLibre -= iC;
        for(int i=0;i<128;i++)
        {
            if(mp[i]==0&&iC>0)
            {
                mp[i]=iP;
                fifoP.insert(make_pair(i,iP));
                iC--;
            }
        }
        //cout << "Se asignaron los marcos de página " <<
        for(int k = 0; k < 2048; k++) // llenamos la memoria
        {
            if(m[k] == 0 && n > 0)
            {
                m[k] = iP;
                n--;
                
            }
        }
        //int aux = 0;
    
        //for(auto it = fifoP.begin(); it != fifoP.end(); ++it)
        //{
          //  if(iP == it->second)
            //{
              //  aux = it->first;
            //}
            //else
            //{
              //  cout <<
            //}
        //}
        int temp, mayor;
        int iC = 0;
        int iC2 = 0;
        int j;
        for(j=0; j<128; j++)
        {
            if(mp[j] == iP)
            {
                if(iC==0)
                {
                    temp = j;
                    iC++;
                }
                if(mp[j+1] != iP)
                {
                    if(iC2==0)
                    {
                        mayor = j;
                        iC2++;
                    }
                }
            }
        }
        cout << "Se asignaron los marcos de página " << temp << " - " << mayor << " al proceso " << iP << endl;
        
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
            iTimeStamp++;
            
        }
        if(cOpc=='P')
        {
            archivoLeer >> iN >> iP;
            cout << "P " << iN << " " << iP << endl;
            cout << "Asignar " << iN << " bytes al proceso " << iP << endl;
            colocarProceso(iN, iP);
        }
        if(cOpc=='A')
        {
            archivoLeer >> iD >> iP >> iM;
            cout << "A " << iD << " " << iP << " " << iM << endl;
            cout << "Obtener la dirección real correspondiente a la dirección virtual " << iD << "del proceso " << iP << endl;
            int result = mostrarDireccionReal(iP, iD);
            cout << "Dirección virtual: " << iD << " Direccion real: " << result << endl;
            iTimeStamp++;

        }
        if(cOpc=='L')
        {
            archivoLeer >> iP;
            cout << "L " << iP << endl;
            cout << "Liberar los marcos de página ocupados por el proceso " << iP << endl;
            iTimeStamp++;
            liberaProceso(iP);
        }
        if(cOpc=='F')
        {
            cout << 'F' << endl;
        }
        if(cOpc=='E')
        {
            imprimirArreglo();
            imprimirMapa();
        }
    }
    
}

int main() {

    iniciarArchivo();
    
    return 0;
}
