//cpu.h
//------		#change cpu.h file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define RAT_SIZE 16
#define PRF_SIZE 24

struct renameTable {
    int slot_id;
    int src_bit;
};

struct Physical_Register_File{
    int value;
    int isFree;
}
	

typedef struct APEX_CPU {

 /*Rename Table*/
    struct renameTable renameTable[16];

int ARF[16];

int Physical_Register_File PRF[24];


}


/***************************************** RenameTable functions *********************************************/
int initializeRenameTable(APEX_CPU *cpu) {
  for (int i = 0; i < RAT_SIZE; i++) {
    cpu->renameTable[i].value = -1;
    cpu->renameTable[i].zFlag = -1;
  }

  return 0;
}

int registerRenaming(APEX_CPU *cpu);

void print_rename_table_entries(APEX_CPU *cpu);
----------------------------------------------------------------------------------------------------------------
Rename.c
---------
registerRenaming(APEX_CPU *cpu){

int initializeRenameTable(APEX_CPU *cpu) {
  for (int i = 0; i < RNT_SIZE; i++) {
    cpu->renameTable[i].slot_id = -1;
    cpu->renameTable[i].src_bit = -1;
  }

  return 0;
}

int getRenamedRegisterForSrcReg(int rs, APEX_CPU *cpu) {
  if (cpu->renameTable[rs].src_bit == -1) {  #means no renaming happened
      return cpu->ARF[rs];
    }else {
       if(cpu->renameTable[rs].src_bit){  #src_bit=1 PRF  
         return cpu->PRF[rs].value;  
        else {
         return cpu->ARF[rs];
 for (int j = 0; j < URF_SIZE; j++) {
    if (cpu->unifiedRF[j].isAvailable) {
      cpu->renameTable[rd].value = j;
      cpu->unifiedRF[j].isAvailable = 0;
      cpu->unifiedRF[j].destLock = 1;
      break;
    }

  }



int getRenamedRegisterForDestReg(int rd, APEX_CPU *cpu) {
  for (int j = 0; j < PRF_SIZE; j++) {
    if (cpu->PRF[j].isFree) {
      cpu->renameTable[rd].slot_id = j;
      cpu->renameTable[rd].src_bit=1;
      cpu->PRF[j].isFree = 0;
      
      break;
    }
  }

  return cpu->renameTable[rd].slot_id;
}

int registerRenaming(APEX_CPU *cpu) {
  CPU_Stage *stage = &cpu->stage[DRF];
  stage->rs1_value =  getRenamedRegisterForSrcReg(stage->rs1, cpu) 
  stage->rs2_value = getRenamedRegisterForSrcReg(stage->rs2, cpu) 
  stage->rd = stage->rd != -1 ? getRenamedRegisterForDestReg(stage->rd, cpu) : -1;

  return 0;
}





