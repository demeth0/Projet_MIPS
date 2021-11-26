#ifndef OPERATION_CODE_DEFINES
#define OPERATION_CODE_DEFINES


/*X_CODE correspond au binaire de l'instruction en hexadecimal*/

/*100000*/
#define ADD_ID   0
#define ADD_CODE 0x20


/*001000*/
#define ADDI_ID   1
#define ADDI_CODE 0x08

/*100100*/
#define AND_ID   2
#define AND_CODE 0x24

/*000100*/
#define BEQ_ID   3
#define BEQ_CODE 0x04

/*000111*/
#define BGTZ_ID   4
#define BGTZ_CODE 0x07

/*000101*/
#define BLEZ_ID   5
#define BLEZ_CODE 0x06

/*000111*/
#define BNE_ID   6
#define BNE_CODE 0x05

/*011010*/
#define DIV_ID   7
#define DIV_CODE 0x1A

/*000010*/
#define J_ID     8
#define J_CODE 0x02

/*000011*/
#define JAL_ID   9
#define JAL_CODE 0x03


/*001000*/
#define JR_ID   10
#define JR_CODE 0x08

/*001111*/
#define LUI_ID   11
#define LUI_CODE 0x0F

/*100011*/
#define LW_ID   12
#define LW_CODE 0x23

/*010000*/
#define MFHI_ID   13
#define MFHI_CODE 0x10

/*010010*/
#define MFLO_ID   14
#define MFLO_CODE 0x12

/*011000*/
#define MULT_ID   15
#define MULT_CODE 0x18

/*000000*/
#define NOP_ID   16
#define NOP_CODE 0x00

/*100101*/
#define OR_ID   17
#define OR_CODE 0x25

/*000010*/
#define ROTR_ID   18
#define ROTR_CODE 0x02

/*000000*/
#define SLL_ID   19
#define SLL_CODE 0x00

/*101010*/
#define SLT_ID   20
#define SLT_CODE 0x2A

/*000010*/
#define SRL_ID   21
#define SRL_CODE 0x02

/*100010*/
#define SUB_ID   22
#define SUB_CODE 0x22

/*101011*/
#define SW_ID   23
#define SW_CODE 0x2B

/*001100*/
#define SYSCALL_ID   24
#define SYSCALL_CODE 0x0C

/*100110*/
#define XOR_ID   25
#define XOR_CODE 0x26
#endif