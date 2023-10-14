#include "mos6502_operation.h"

#include <sstream>

#include <fmt/core.h>

namespace libemu::dasm
{
	std::string mos6502_operation::to_string() const noexcept
	{
		const static std::string instruction_names[]
			{
				"adc",
				"and",
				"asl",
				"bit",
				"bpl",
				"bmi",
				"bvc",
				"bvs",
				"bcc",
				"bcs",
				"bne",
				"beq",
				"brk",
				"cmp",
				"cpx",
				"cpy",
				"dec",
				"eor",
				"clc",
				"sec",
				"cli",
				"sei",
				"clv",
				"cld",
				"sed",
				"inc",
				"jmp",
				"jsr",
				"lda",
				"ldx",
				"ldy",
				"lsr",
				"nop",
				"ora",
				"tax",
				"txa",
				"dex",
				"inx",
				"tay",
				"tya",
				"dey",
				"iny",
				"rol",
				"ror",
				"rti",
				"rts",
				"sbc",
				"sta",
				"txs",
				"tsx",
				"pha",
				"pla",
				"php",
				"plp",
				"stx",
				"sty",
				"alr",
				"anc",
				"anc2",
				"ane",
				"arr",
				"dcp",
				"isc",
				"las",
				"lax",
				"lxa",
				"rla",
				"rra",
				"sax",
				"sbx",
				"sha",
				"shx",
				"shy",
				"slo",
				"sre",
				"tas",
				"usbc",
				"jam",
			};
		const static fmt::format_string<int32_t> addressing_mode_formats[]
			{
				"",
				"a",
				"#${:02x}",
				"${:02x}",
				"${:02x},x",
				"${:02x},y",
				"*{:+d}",
				"${:04x}",
				"${:04x},x",
				"${:04x},y",
				"(${:04x})",
				"(${:02x},x)",
				"(${:02x}),y",
			};
		std::stringstream ss{};
		if (long relative_operator{ static_cast<long>(static_cast<int8_t>(operator_ + 2)) };
			addressing_mode == libemu::dasm::mos6502_operation::relative)
			ss << instruction_names[instruction] << " "
			   << fmt::vformat(addressing_mode_formats[addressing_mode].get(),
				   fmt::make_format_args(relative_operator));
		else
			ss << instruction_names[instruction] << " "
			   << fmt::vformat(addressing_mode_formats[addressing_mode].get(), fmt::make_format_args(operator_));
		return ss.str();
	}

	mos6502_operation::mos6502_operation(const libemu::dasm::mos6502_operation::instructions instruction,
		const unsigned long operator_,
		const libemu::dasm::mos6502_operation::addressing_modes addressing_mode,
		const unsigned long instruction_length,
		const unsigned long execution_time) noexcept
		: instruction{ instruction }, operator_{ operator_ }, addressing_mode{ addressing_mode },
		instruction_length{ instruction_length }, execution_time{ execution_time }
	{
	}
}
