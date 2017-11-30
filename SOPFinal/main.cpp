#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_map>

using namespace std;

//Declaracion de variables globales
unordered_map<int,int> fifoP;
int swa[4096]={0};
int mp[127]={0};
int m[2048]={0};
int iPageFaults = 0;
int iTimeStamp = 0;
int iLibre=128;
int iSwaps = 0;
int iC=0;

//Funciones para imprimir resultados
void imprimirArreglo()
{
  cout << "--ARREGLO--" << endl;
  for(int i=0;i<128;i++)
  {
    cout << " " << mp[i] << endl;
  }
}

void imprimirMapa()
{
  cout << "--MAPA--" << endl;
  for(auto it = fifoP.begin(); it != fifoP.end(); ++it)
  {
    cout << " " << it->first << ":" << it->second << endl;
  }
}

//Funcion para mostrar la direccion real, dada la direccion virtual
int mostrarDireccionReal(int iP, int iD)
{
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

//Funcion para liberar un proceso
void liberaProceso(int iP)
{
  int temp = -1;
  int mayor = -1;
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
  if(temp != -1)
  {
    cout << "Se liberan los marcos de página de memoria real: " << temp << " - " << mayor << endl;
  }

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

  temp = -1;
  mayor = -1;
  iC = 0; iC2 = 0;

  //Se liberan marcos de página del arreglo de swapping en caso de que el proceso se encuentre ahí
  for(j=0; j<4096; j++)
  {
    if(swa[j] == iP)
    {
      if(iC==0)
      {
        temp = j;
        iC++;
      }
      if(swa[j+1] != iP)
      {
        if(iC2==0)
        {
          mayor = j;
          iC2++;
        }
      }
    }
  }
  if(temp != -1)
  {
    cout << "Se liberan los marcos " << temp << " - " << mayor << " del área de swapping" << endl;
  }

  for(int i=0;i<4096;i++)
  {
    if(swa[i]==iP)
    {
      swa[i]=0;
    }
  }
}

//Funciona para realizar el swap con FIFO
void swapFifo(int iN, int iP)
{
  int index=0;
  int temp3=0;

  iC = ceil(iN/16.0);
  iLibre -= iC;

  for(int i=0;i<iC;i++)
  {
    //Trae la última posición del mapa
    unordered_map<int,int>::iterator lastElement;
    for(auto it = fifoP.begin(); it != fifoP.end(); ++it)
    {
      lastElement = it;
    }

    //Swap
    swa[i]=iP;
    index = lastElement->first;
    temp3 = mp[index];
    mp[index]=swa[i];
    swa[i]=temp3;
    fifoP.erase(index);
    fifoP.insert(make_pair(index,iP));
    iSwaps++;
  }
}

//Funcion para colocar un proceso
void colocarProceso(int iN, int iP)
{
  iC = ceil(iN/16.0);
  iTimeStamp += iC;
  int n = iN;

  //Si hay espacio libre en el arreglo para colocar el proceso
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

    //Se llena el arreglo
    for(int k = 0; k < 2048; k++)
    {
      if(m[k] == 0 && n > 0)
      {
        m[k] = iP;
        n--;
      }
    }

    int temp = -1, mayor = -1;
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
  //Si no hay espacio libre en el arreglo para colocar el proceso, se utiliza FIFO y LRU
  else
  {
    // cout << "--FIFO--" << endl;
    swapFifo(iN,iP);
    // imprimirArreglo();
  }
}

//Funciones para la llamadas de comandos
void caseP(int iN, int iP)
{
  cout << "P " << iN << " " << iP << endl;
  cout << "Asignar " << iN << " bytes al proceso " << iP << endl;
  colocarProceso(iN, iP);
}

void caseA(int iD, int iP, int iM)
{
  cout << "A " << iD << " " << iP << " " << iM << endl;
  if(iM == 0)
  {
    cout << "Obtener la dirección real correspondiente a la dirección virtual " << iD << " del proceso " << iP << endl;
  }
  if(iM == 1)
  {
    cout << "Obtener la dirección real correspondiente a la dirección virtual " << iD << " del proceso " << iP << " y modificar dicha dirección" << endl;
    cout << "Se modificó la dirección " << iD << " del proceso " << iP << endl;
  }
  int result = mostrarDireccionReal(iP, iD);
  cout << "Dirección virtual: " << iD << " Direccion real: " << result << endl;
  iTimeStamp++;
}

void caseL(int iP)
{
  cout << "L " << iP << endl;
  cout << "Liberar los marcos de página ocupados por el proceso " << iP << endl;
  iTimeStamp++;
  liberaProceso(iP);
}

int main()
{
  //Declaracion de variables
  ifstream archivoLeer;
  int iD,iN,iP,iM;
  string sReplica;
  char cOpc;

  //Leer archivo de prueba
  archivoLeer.open("prueba.txt");

  //Loop para la llamada de comandos
  while(archivoLeer>>cOpc)
  {
    switch (cOpc) {
      case 'C':
        getline(archivoLeer,sReplica);
        cout << sReplica << endl;
        iTimeStamp++;
        break;

      case 'P':
        archivoLeer >> iN >> iP;
        caseP(iN, iP);
        break;

      case 'A':
        archivoLeer >> iD >> iP >> iM;
        caseA(iD, iP, iM);
        break;

      case 'L':
        archivoLeer >> iP;
        caseL(iP);
        break;

      case 'F':
        cout << 'F' << endl;
        break;

      case 'E':
        imprimirArreglo();
        imprimirMapa();
        break;
    }
  }
  return 0;
}
