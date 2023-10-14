#pragma once

#include "mos6502_operation.h"

#include "../disassembler.h"

namespace libemu::dasm
{
	class mos6502_disassembler final : public disassembler<mos6502_operation>
	{
	public:
		mos6502_operation disassemble_internal(const byte* bytes, offset begin_offset, offset& end_offset) noexcept override;

		operation disassemble(const byte* bytes, offset begin_offset, offset& end_offset) noexcept override;

		~mos6502_disassembler() noexcept override;
	};
}
