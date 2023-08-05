#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <disassembler.h>
#include <mos6502/mos6502_disassembler.h>

#include <fmt/core.h>

namespace
{
	std::vector<std::string> split(const std::string& str, const char delim)
	{
		std::vector<std::string> result{};
		std::stringstream ss{ str };
		std::string token{};
		while (getline(ss, token, delim))
			result.push_back(token);
		return result;
	}

	enum architectures
	{
		unset,
		mos6502
	};
}

int main()
{
	std::cout << "libemu Debug Console" << std::endl
			  << "|-> Type \"help\" for a list of commands/architectures" << std::endl;
	architectures architecture{};
	while (true)
	{
		std::string buffer{};
		std::vector<std::string> tokens{};
		std::cout << "> ";
		getline(std::cin, buffer);
		tokens = split(buffer, ' ');
		if (tokens.empty())
			continue;
		if (tokens.at(0) == "q" || tokens.at(0) == "quit")
			break;
		if (tokens.at(0) == "h" || tokens.at(0) == "help")
		{
			std::cout << "< List of commands (case sensitive)" << std::endl
					  << "|->quit (q): Quit the debug console" << std::endl
					  << "|->help (h): Show the list of commands/architectures" << std::endl
					  << "|->arch (a): Set architecture" << std::endl
					  << "|->dasm (d): Disassemble instructions in hex format (individually)" << std::endl
					  << "   |-> e.g. dasm EA 69 FF" << std::endl
					  << std::endl
					  << "< List of architectures" << std::endl
					  << "mos6502: MOS Technology 6502" << std::endl;
			continue;
		}
		if (tokens.at(0) == "a" || tokens.at(0) == "arch")
		{
			if (tokens.size() < 2)
			{
				std::cout << "< Too few arguments" << std::endl;
				continue;
			}
			if (tokens.at(1) == "mos6502")
				architecture = mos6502;
			else if (tokens.at(1) == "unset")
			{
				architecture = unset;
				std::cout << "< Unset architecture" << std::endl;
				continue;
			}
			else
			{
				std::cout << "< Unknown architecture" << std::endl;
				continue;
			}
			std::cout << "< Set architecture to \"" << tokens.at(1) << "\"" << std::endl;
		}
		if (tokens.at(0) == "d" || tokens.at(0) == "dasm")
		{
			if (architecture == unset)
			{
				std::cout << "< Architecture not set" << std::endl;
				continue;
			}
			if (tokens.size() < 2)
			{
				std::cout << "< Too few arguments" << std::endl;
				continue;
			}
			switch (architecture)
			{
			case mos6502:
			{
				static std::string instruction_names[]
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
				static fmt::format_string<int32_t> addressing_mode_formats[]
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
				libemu::dasm::mos6502_disassembler disassembler{};
				std::vector<libemu::dasm::mos6502_operation> operations{};
				std::unique_ptr<libemu::byte[]> memory{ std::make_unique<libemu::byte[]>(tokens.size() - 1) };
				for (unsigned long i{ 1 }; i < tokens.size(); i++)
					memory[i - 1] = std::stoi(tokens.at(i), nullptr, 16);
				for (libemu::offset i{ static_cast<libemu::offset>(tokens.size() - 2) }, j{}; j < tokens.size() - 1;
					 i = static_cast<libemu::offset>(tokens.size() - 2))
				{
					operations.push_back(disassembler.disassemble_internal(memory.get(), j, i));
					j = i + 1;
				}
				for (unsigned long i{}; i < operations.size(); i++)
				{
					if (int32_t operator_{ static_cast<int32_t>(static_cast<int8_t>(operations.at(i).operator_ + 2)) };
						operations.at(i).addressing_mode == libemu::dasm::mos6502_operation::relative)
						std::cout << instruction_names[operations.at(i).instruction] << " " << fmt::vformat(
							addressing_mode_formats[operations.at(i).addressing_mode].get(),
							fmt::make_format_args(operator_)) << std::endl;
					else
						std::cout << instruction_names[operations.at(i).instruction] << " " << fmt::vformat(
							addressing_mode_formats[operations.at(i).addressing_mode].get(),
							fmt::make_format_args(operations.at(i).operator_)) << std::endl;
				}
				break;
			}
			}
		}
	}
	return 0;
}
