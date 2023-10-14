#pragma once

#include <operation.h>
#include <type_define.h>

namespace libemu::dasm
{
	template <operational T>
	class disassembler
	{
	public:
		/**
		 * Disassemble one operation
		 * @param bytes Bytes to disassemble
		 * @param begin_offset Byte offset to where the operation starts
		 * @param end_offset [IN] Where the bytes end [OUT] Byte offset after the operation
		 * @return The disassembled operation
		 */
		virtual T disassemble_internal(const byte* bytes, offset begin_offset, offset& end_offset) noexcept = 0;

		/**
		 * Disassemble one operation
		 * @param bytes Bytes to disassemble
		 * @param begin_offset Byte offset to where the operation starts
		 * @param end_offset [IN] Where the bytes end [OUT] Byte offset after the operation
		 * @return The disassembled operation
		 */
		virtual operation disassemble(const byte* bytes, offset begin_offset, offset& end_offset) noexcept = 0;

		virtual ~disassembler() noexcept = default;
	};
}
