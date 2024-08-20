#include "ExcelGet.h"





void* Pool_DIDList(uint32_t DIDMAXnumber)
{
  DIDList* DIDListHead=(DIDList*)malloc(sizeof(DIDList));
  DIDList* tailptr=DIDListHead;
  //RigntMovie->light=DIDListHead;
  for(uint32_t i=1;i<DIDMAXnumber;i++)
 {
    tailptr->right=(DIDList*)malloc(sizeof(DIDList));
    tailptr->right->right=nullptr;
    tailptr=tailptr->right;
  }
   tailptr->right=DIDListHead;
   return DIDListHead;
}

void* WriteDataToPool(DIDList* ptr,const DIDList* Now)
{
  ptr->DID=Now->DID;
  ptr->DataSize=Now->DataSize;
  ptr->DIDSessionSupport=Now->DIDSessionSupport;
  memcpy(ptr->DIDDefaultData,Now->DIDDefaultData,Now->DataSize*2+1);
  if(Now->DIDWriteON==1&&Now->WriteONPtr)
  {
     ptr->DIDWriteON=1;
     ptr->WriteONPtr=(WriteDiDStruct*)malloc(sizeof(WriteDiDStruct));
     memcpy(ptr->WriteONPtr->DataMax,Now->WriteONPtr->DataMax,Now->DataSize*2+1);
     memcpy(ptr->WriteONPtr->DataMin,Now->WriteONPtr->DataMin,Now->DataSize*2+1);
     ptr->DID2E_NRC31=Now->DID2E_NRC31;
     ptr->DID2E_NRC33=Now->DID2E_NRC33;
  }
  else
  {
      ptr->DIDWriteON=0;
      ptr->WriteONPtr=nullptr;
  }

      return ptr->right;
}

void* GetDatatoPool(DIDList* Now,const DIDList* ptr)
{
    Now->DID=ptr->DID;
    Now->DataSize=ptr->DataSize;
    Now->DIDSessionSupport=ptr->DIDSessionSupport;
    memcpy(Now->DIDDefaultData,ptr->DIDDefaultData,ptr->DataSize*2+1);
    if(ptr->DIDWriteON==1&&ptr->WriteONPtr)
    {
       Now->DIDWriteON=1;
       Now->WriteONPtr=(WriteDiDStruct*)malloc(sizeof(WriteDiDStruct));
       memcpy(Now->WriteONPtr->DataMax,ptr->WriteONPtr->DataMax,ptr->DataSize);
       memcpy(Now->WriteONPtr->DataMin,ptr->WriteONPtr->DataMin,ptr->DataSize);
       Now->DID2E_NRC31=ptr->DID2E_NRC31;
       Now->DID2E_NRC33=ptr->DID2E_NRC33;
    }

    return ptr->right;
}
 void* DelPOOL(DIDList** Headptr)
    {
        DIDList* First = (*Headptr);
        if(!First)
            return NULL;
        do
        {
            DIDList* nextptr = First;
            First = First->right;
            free(nextptr);
        } while (First != (*Headptr));
        *Headptr = NULL;
        return (*Headptr);
    }

