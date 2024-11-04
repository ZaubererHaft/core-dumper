.section .text
.global CreateDump
.syntax unified

// IN: R0 - ptr to registers buffer
// IN: R1 - ptr to stack buffer
// IN: R2 - ptr to data buffer
// IN: R3 - ptr to meminfo struct / array (with stack start addr, stack end addr, data start addr, data end addr)

// Remark: R0 - R3 scratch register (caller-saved)
// R4-R11 callee-saved
CreateDump:
  PUSH {R4, R5}

  // first save R0 because using it as base in the STMIA instruction would be unpredictable
  STR R0, [R0]
  ADD R0, #4

  //R1-R12 and write increment back
  STMIA R0!, {R1-R12}

  //R13 / SP
  STR SP, [R0]
  ADD R0, #4

  //R14 / LR
  STR LR, [R0]
  ADD R0, #4

  // PC will freeze here
  MOV R4, PC
  STR R4, [R0]
  ADD R0, #4

  // Now: Store the program status register
  MRS R4, XPSR
  STR R4, [R0]

//---------------------Copy Full Stack-----------------------------------
  LDR R4, [R3, #0]     // stack start
  LDR R5, [R3, #4]     // stack end

loop_stack:
  LDR R0, [R4]     //copy word to R0
  STR R0, [R1]     //store R0 to address at R1 / stack buffer
  ADD R1, 4
  ADD R4, 4
  CMP R4, R5
  BLT loop_stack

//---------------------Copy data segment -----------------------------------
  LDR R4, [R3, #8]    //data start
  LDR R5, [R3, #12]   //data end

loop_data:
  LDR R0, [R4]
  STR R0, [R2]
  ADD R2, 4
  ADD R4, 4
  CMP R4, R5
  BLT loop_data

  POP {R4, R5}

  BX LR