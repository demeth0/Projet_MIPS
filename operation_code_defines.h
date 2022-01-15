#ifndef OPERATION_CODE_DEFINES
#define OPERATION_CODE_DEFINES


/*X_CODE correspond au binaire de l'instruction en hexadecimal*/
/*
X_ID correspond a un nombre unique pour définir une instruction
     les deux bits de poid forts correspondent au type d'instruction
*/
#define OPCODE_TYPE_MASK 0xC0
#define OPCODE_TYPE_R    0xC0 /*11*/
#define OPCODE_TYPE_I    0x80 /*10*/
#define OPCODE_TYPE_J    0x40 /*01*/



/*100000*/
#define ADD_ID   (0x00|OPCODE_TYPE_R)
#define ADD_CODE 0x20


/*001000*/
#define ADDI_ID   (0x01|OPCODE_TYPE_I)
#define ADDI_CODE 0x08

/*100100*/
#define AND_ID   (0x02|OPCODE_TYPE_R)
#define AND_CODE 0x24

/*000100*/
#define BEQ_ID   (0x03|OPCODE_TYPE_I)
#define BEQ_CODE 0x04

/*000111*/
#define BGTZ_ID   (0x04|OPCODE_TYPE_I)
#define BGTZ_CODE 0x07

/*000101*/
#define BLEZ_ID   (0x05|OPCODE_TYPE_I)
#define BLEZ_CODE 0x06

/*000111*/
#define BNE_ID   (0x06|OPCODE_TYPE_I)
#define BNE_CODE 0x05

/*011010*/
#define DIV_ID   (0x07|OPCODE_TYPE_R)
#define DIV_CODE 0x1A

/*000010*/
#define J_ID     (0x08|OPCODE_TYPE_J)
#define J_CODE 0x02

/*000011*/
#define JAL_ID   (0x09|OPCODE_TYPE_J)
#define JAL_CODE 0x03


/*001000*/
#define JR_ID   (0x0A|OPCODE_TYPE_R)
#define JR_CODE 0x08

/*001111*/
#define LUI_ID   (0x0B|OPCODE_TYPE_I)
#define LUI_CODE 0x0F

/*100011*/
#define LW_ID   (0x0C|OPCODE_TYPE_I)
#define LW_CODE 0x23

/*010000*/
#define MFHI_ID   (0x0D|OPCODE_TYPE_R)
#define MFHI_CODE 0x10

/*010010*/
#define MFLO_ID   (0x0E|OPCODE_TYPE_R)
#define MFLO_CODE 0x12

/*011000*/
#define MULT_ID   (0x0F|OPCODE_TYPE_R)
#define MULT_CODE 0x18

/*000000*/
#define NOP_ID   (0x10|OPCODE_TYPE_R)
#define NOP_CODE 0x00

/*100101*/
#define OR_ID   (0x11|OPCODE_TYPE_R)
#define OR_CODE 0x25

/*000010*/
#define ROTR_ID   (0x12|OPCODE_TYPE_R)
#define ROTR_CODE 0x02

/*000000*/
#define SLL_ID   (0x13|OPCODE_TYPE_R)
#define SLL_CODE 0x00

/*101010*/
#define SLT_ID   (0x14|OPCODE_TYPE_R)
#define SLT_CODE 0x2A

/*000010*/
#define SRL_ID   (0x15|OPCODE_TYPE_R)
#define SRL_CODE 0x02

/*100010*/
#define SUB_ID   (0x16|OPCODE_TYPE_R)
#define SUB_CODE 0x22

/*101011*/
#define SW_ID   (0x17|OPCODE_TYPE_I)
#define SW_CODE 0x2B

/*001100*/
#define SYSCALL_ID   (0x18|OPCODE_TYPE_J)
#define SYSCALL_CODE 0x0C

/*100110*/
#define XOR_ID   (0x19|OPCODE_TYPE_R)
#define XOR_CODE 0x26

#define UNKNOWN_ID 0xFF
#define HALT_ID 0xFE
#endif