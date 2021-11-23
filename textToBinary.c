#include "textToBin.h"
#include "operation_code_defines.h"

/*
On suppose :
	le texte instruction est valide
	la struct Instruction est initialisée
*/
void textInstructionToOpcode(char textInstruction, Instruction *instruction){
	char [8] opcode;
	getInstruction(textInstruction,opcode);

	if(opcode[1]=='\0' && opcode[0]=='j'){
		/* alors forcément instruction 'j' 
			j -> 6 premiers bits a 000010
			donc code[0] = 000010xx
		*/
		instruction->code[0] = (instruction->code[0]&0x03) + (J_CODE<<2);
		instruction->id = J_ID;
	}else if(opcode[2]=='\0'){
		/*
			or
			lw
			jr
			sw
		*/
	}else if(opcode[3]=='\0'){
		/*
		add
		and
		beq
		bne
		div		
		jal
		lui
		nop
		sll
		slt
		srl
		sub
		xor*/
	}else if(opcode[4]=='\0'){
		/*rotr
		addi
		bgtz
		blez
		mfhi
		mflo
		mult*/
	}else{
		/* syscall */
	}
}