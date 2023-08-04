#pragma once

#include <operation.h>
#include <type_define.h>

namespace libemu::dasm
{
	class disassembler
	{
	public:
		/**
		 * Disassemble one operation
		 * @param bytes Bytes to disassemble
		 * @param begin_offset Byte offset to where the operation starts
		 * @param end_offset Byte offset after the operation
		 * @return The disassembled operation
		 */
		virtual operation disassemble(const byte* bytes, offset begin_offset, offset& end_offset) noexcept = 0;
	};
}
