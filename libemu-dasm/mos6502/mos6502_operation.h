#pragma once

#include <operation.h>
#include <string>
#include <type_define.h>

namespace libemu::dasm
{
	class mos6502_operation final : operation
	{
	public:
		enum instructions
		{
			adc,    // Add with carry
			and_,   // Bitwise and with accumulator
			asl,    // Arithmetic shift left
			bit,    // Test bits
			bpl,    // Branch on plus
			bmi,    // Branch on minus
			bvc,    // Branch on overflow clear
			bvs,    // Branch on overflow set
			bcc,    // Branch on carry clear
			bcs,    // Branch on carry set
			bne,    // Branch on not equal
			beq,    // Branch on equal
			brk,    // Break
			cmp,    // Compare accumulator
			cpx,    // Compare X register
			cpy,    // Compare Y register
			dec,    // Decrement memory
			eor,    // Bitwise exclusive or
			clc,    // Clear carry
			sec,    // Set carry
			cli,    // Clear interrupt
			sei,    // Set interrupt
			clv,    // Clear overflow
			cld,    // Clear decimal
			sed,    // Set decimal
			inc,    // Increment memory
			jmp,    // Jump
			jsr,    // Jump to subroutine
			lda,    // Load accumulator
			ldx,    // Load X register
			ldy,    // Load Y register
			lsr,    // Logical shift right
			nop,    // No operation
			ora,    // Bitwise or with accumulator
			tax,    // Transfer A to X
			txa,    // Transfer X to A
			dex,    // Decrement X
			inx,    // Decrement X
			tay,    // Transfer A to Y
			tya,    // Transfer Y to A
			dey,    // Decrement Y
			iny,    // Increment Y
			rol,    // Rotate left
			ror,    // Rotate right
			rti,    // Return from interrupt
			rts,    // Return from subroutine
			sbc,    // Subtract with carry
			sta,    // Store accumulator
			txs,    // Transfer X to stack ptr
			tsx,    // Transfer stack ptr to X
			pha,    // Push accumulator
			pla,    // Pull accumulator
			php,    // Push processor status
			plp,    // Pull processor status
			stx,    // Store X register
			sty,    // Store Y register
			// "Illegal" Instructions
			// https://www.masswerk.at/nowgobang/2021/6502-illegal-opcodes
			alr,
			anc,
			anc2,
			ane,
			arr,
			dcp,
			isc,
			las,
			lax,
			lxa,
			rla,
			rra,
			sax,
			sbx,
			sha,
			shx,
			shy,
			slo,
			sre,
			tas,
			usbc,
			jam
		};

		enum addressing_modes
		{
			implied,		// clc
			accumulator,	// lsr a
			immediate,		// lda #10
			zero_page,		// lda $00
			zero_page_x,	// sty $10, x
			zero_page_y,	// ldx $10, y
			relative,		// bne *+4
			absolute,		// jmp $1234
			absolute_x,		// sta $3000, x
			absolute_y,		// and $4000, y
			indirect,		// jmp ($fffc)
			indirect_x,		// lda ($40, x)
			indirect_y, 	// lda ($40), y
		};

		instructions instruction;
		unsigned long operator_;
		addressing_modes addressing_mode;
		unsigned long instruction_length;
		unsigned long execution_time;

		[[nodiscard]] std::string to_string() const noexcept;

		mos6502_operation(instructions instruction,
			unsigned long operator_,
			addressing_modes addressing_mode,
			unsigned long instruction_length,
			unsigned long execution_time) noexcept;
	};
}
