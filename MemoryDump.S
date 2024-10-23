.section .text
.global CreateDump
.syntax unified

# IN: R0 - ptr to registers
# IN: R1 - ptr to stack
# IN: R2 - ptr to data

#Remark: R0 - R3 scratch register (caller-saved)
#R4 is callee-saved

CreateDump:
  PUSH {R4}
  STR R0, [R0, #0]
  STR R1, [R0, #4]
  STR R2, [R0, #8]
  STR R3, [R0, #12]
  STR R4, [R0, #16]
  STR R5, [R0, #20]
  STR R6, [R0, #24]
  STR R7, [R0, #28]

  ADD R0, 32

  STR R8,  [R0, #0]
  STR R9,  [R0, #4]
  STR R10, [R0, #8]
  STR R11, [R0, #12]
  STR R12, [R0, #16]
  STR R13, [R0, #20]
#goal: freeze at the following instruction
  STR R14, [R0, #24]
  MOV R4, PC
# roll PC back to instruction above (2 instruction => 2 * 4 Byte)
  SUB R4, 8
  STR R4, [R0, #28]

  ADD R0, 32

# Store the program status register currently with hack to incorporate default value
  MOV R4, 1
  LSL R4, 24
  MRS R3, XPSR
  ORR R3, R4
  STR R3, [R0, #0]

#---------------------Copy Full Stack-----------------------------------
  LDR R3, =st_StackStartAddress
  LDR R4, =st_StackEndAddress
  LDR R3, [R3]
  LDR R4, [R4]

loop_stack:
  LDR R0, [R3]
  STR R0, [R1]
  ADD R1, 4
  ADD R3, 4
  CMP R3, R4
  BLT loop_stack

#---------------------Copy data segment -----------------------------------
  LDR R3, =st_DataStartAddress
  LDR R4, =st_DataEndAddress
  LDR R3, [R3]
  LDR R4, [R4]

loop_data:
  LDR R0, [R3]
  STR R0, [R2]
  ADD R2, 4
  ADD R3, 4
  CMP R3, R4
  BLT loop_data

  POP {R4}

  BX LR
