#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define ROB_SIZE 12

void print_rob_instruction(struct ROBEntry stage) {
  if (strcmp(stage.opcode, "STORE") == 0) {
    
    printf("%5s,R%-2d,R%-2d,#%-2d  ", stage.opcode, stage.rs1, stage.rs2, stage.imm);
  }

  if (
      strcmp(stage.opcode, "LOAD") == 0 ||
      strcmp(stage.opcode, "ADDL") == 0 ||
      strcmp(stage.opcode, "SUBL") == 0 ||
      
      ) {
   
    printf("%5s,R%-2d,R%-2d,#%-2d ", stage.opcode, stage.rd, stage.rs1, stage.imm);
  }

  if (strcmp(stage.opcode, "MOVC") == 0) {
    
    printf("%5s,R%-2d,R%-2d,#%-2d", stage.opcode, stage.rd, stage.imm);
  }

  if (strcmp(stage.opcode, "JUMP") == 0) {
   
    printf("%5s,R%-2d,%6s", stage.opcode, stage.rs1, stage.imm);
  }

  if (strcmp(stage.opcode, "HALT") == 0) {
    printf("%5s\t\t\t%31s", stage.opcode);
  }

  if (
      strcmp(stage.opcode, "BZ") == 0 ||
      strcmp(stage.opcode, "BNZ") == 0
      ) {
    printf("%5s,#%-2d\t\t", stage.opcode, stage.imm);
  }

  if (strcmp(stage.opcode, "NOP") == 0) {
    printf("%5s\t\t\t", stage.opcode);
  }

  if (
      strcmp(stage.opcode, "ADD") == 0 ||
      strcmp(stage.opcode, "SUB") == 0 ||
      strcmp(stage.opcode, "MUL") == 0 ||
      strcmp(stage.opcode, "AND") == 0 ||
      strcmp(stage.opcode, "EX-OR") == 0 ||
      strcmp(stage.opcode, "OR") == 0
      ) {
   
    printf("\t\t%5s,R%-2d,R%-2d,R%-2d ", stage.opcode, stage.rd, stage.rs1, stage.rs2);
  }
}

void print_rob_entries(APEX_CPU *cpu) {
  printf(" _______________________________________________________________________________\n");
  printf("|\t\t\t\tDetails of ROB State:\t\t\t\t|\n");
  for (int i = 0; i < ROB_SIZE; i++) {
    if (cpu->ROB[i].rd != -1 || cpu->ROB[i].rs1 != -1) {
      printf("|ROB[%d]:\t\t", i);
      print_rob_instruction(cpu->ROB[i]);
      printf("\t|\n");
    }
  }
  printf(" ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");

}


struct ROBEntry createDummyROBEntry() {
  struct ROBEntry dummyROBEntry;

  strcpy(dummyROBEntry.opcode, "");
  dummyROBEntry.rs1 = -1;
  dummyROBEntry.rs2 = -1;
  dummyROBEntry.rd = -1;
  dummyROBEntry.imm = -1;

  dummyROBEntry.result = -1;
  dummyROBEntry.isValidsResultStatus = 0;
  return dummyROBEntry;
}

struct ROBEntry createROBEntry(APEX_CPU *cpu) {
  CPU_Stage *stage = &cpu->stage[FQ];
  struct ROBEntry temp;

  strcpy(temp.opcode, stage->opcode);
  temp.rs1 = stage->rs1;
  temp.rs2 = stage->rs2;
  temp.rd = stage->rd;

  temp.imm = stage->imm;

  temp.pc = stage->pc;
  temp.ar_address = stage->rd;
  temp.excodes = -1;

  temp.isValidsResultStatus = -1;
  if(strcmp(temp.opcode, "HALT") == 0){
    //special handling for HALT.
    temp.isValidsResultStatus = 1;
  }

  temp.result = -1;

  return temp;
}

int insertInROB(APEX_CPU *cpu) {
  struct ROBEntry robEntry = createROBEntry(cpu);

  if (cpu->robRear < ROB_SIZE) {
    cpu->ROB[cpu->robRear] = robEntry;
    cpu->robRear++;
  }

  if (cpu->robRear == ROB_SIZE) {
    cpu->robFull = 1;
  }
  return 0;
}



struct ROBEntry retireROBEntry(APEX_CPU *cpu) {

  struct ROBEntry retiredROBEntry = cpu->ROB[0];

  //shift all elements by 1 left.
  for (int i = 0; i <= cpu->robRear; i++) {
    cpu->ROB[i] = cpu->ROB[i + 1];
  }

  // flush last entry of ROB
  cpu->ROB[cpu->robRear] = createDummyROBEntry();

  cpu->robRear--;
  cpu->robFull = 0;

  updateARF(cpu, retiredROBEntry);

  if(strcmp(retiredROBEntry.opcode, "HALT") == 0){
    cpu->haltRetiredFromROB = 1;
  }

  return retiredROBEntry;
}

int initializeROB(APEX_CPU *cpu) {

  for (int i = 0; i < ROB_SIZE; i++) {
    cpu->ROB[i] = createDummyROBEntry();
  }
  cpu->robRear = 0;
  cpu->robFull = 0;

  return 0;
}

void deleteROBEntryByIndex(APEX_CPU *cpu, int desiredIndex) {
  //shift all elements by 1 left.
  for (int i = desiredIndex; i <= cpu->robRear; i++) {
    cpu->ROB[i] = cpu->ROB[i + 1];
  }
  // flush last entry of ROB
  cpu->ROB[cpu->robRear] = createDummyROBEntry();
}

  return 0;
}