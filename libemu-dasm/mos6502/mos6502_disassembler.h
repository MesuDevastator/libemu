#pragma once

#include "mos6502_operation.h"

#include "../disassembler.h"

namespace libemu::dasm
{
    class mos6502_disassembler final : public disassembler
    {
    public:
        mos6502_operation disassemble_internal(const byte* bytes, offset begin_offset, offset& end_offset) noexcept;

        operation disassemble(const byte* bytes, offset begin_offset, offset& end_offset) noexcept override;
    };
}
