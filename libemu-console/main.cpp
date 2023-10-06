#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <disassembler.h>
#include <mos6502/mos6502_disassembler.h>

#include <fmt/core.h>

namespace
{
	std::vector<std::string> split(std::string_view str)
	{
		std::vector<std::string> result{};
		std::stringstream ss{ static_cast<std::string>(str) };
		std::string token{};
		while (getline(ss, token, ' '))
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
		tokens = split(buffer);
		if (tokens.empty())
			continue;
		else if (tokens.at(0) == "q" || tokens.at(0) == "quit")
			break;
		else if (tokens.at(0) == "h" || tokens.at(0) == "help")
		{
			std::cout << "< List of commands (case sensitive)" << std::endl
					  << "|->quit (q): Quit the debug console" << std::endl
					  << "|->help (h): Show the list of commands/architectures" << std::endl
					  << "|->arch (a): Set architecture" << std::endl
					  << "|->dasm (d): Disassemble instructions in hex format (individually)" << std::endl
					  << "   |-> e.g. dasm EA 69 FF" << std::endl
					  << "< List of architectures" << std::endl
					  << "mos6502: MOS Technology 6502" << std::endl;
		}
		else if (tokens.at(0) == "a" || tokens.at(0) == "arch")
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
		else if (tokens.at(0) == "d" || tokens.at(0) == "dasm")
		{
			if (tokens.size() < 2)
			{
				std::cout << "< Too few arguments" << std::endl;
				continue;
			}
			switch (architecture)
			{
			case mos6502:
			{
				libemu::dasm::mos6502_disassembler disassembler{};
				std::vector<libemu::dasm::mos6502_operation> operations{};
				std::unique_ptr<libemu::byte[]> memory{ std::make_unique<libemu::byte[]>(tokens.size() - 1) };
				for (unsigned long i{ 1 }; i < tokens.size(); i++)
					try
					{
						unsigned long long byte{ std::stoull(tokens.at(i), nullptr, 16) };
						if (byte > 0xff)
							throw std::out_of_range{ "" };
						memory[i - 1] = static_cast<libemu::byte>(byte);
					}
					catch (const std::out_of_range&)
					{
						std::cout << "< Too large byte: " << tokens.at(i) << " (tips: separate bytes)" << std::endl;
						goto loop_while;    // Well, we have to do this
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "< Invalid byte: " << tokens.at(i) << std::endl;
						goto loop_while;    // Well, we have to do this
					}
				for (libemu::offset i{ static_cast<libemu::offset>(tokens.size() - 2) }, j{}; j < tokens.size() - 1;
					 i = static_cast<libemu::offset>(tokens.size() - 2))
				{
					operations.push_back(disassembler.disassemble_internal(memory.get(), j, i));
					j = i + 1;
				}
				unsigned long longest{};
				for (unsigned long i{}; i < operations.size(); i++)
					longest = std::max(operations.at(i).instruction_length, longest);
				for (unsigned long i{}, offset{}; i < operations.size(); i++)
				{
					unsigned long j;
					for (j = offset; j < operations.at(i).instruction_length + offset; j++)
						std::cout << fmt::format("{:02x} ", memory[j]);
					offset = j;
					for (; j < longest; j++)
						std::cout << "   ";
					std::cout << "| " << operations.at(i).to_string() << std::endl;
				}
				break;
			}
			case unset:
				std::cout << "< Architecture not set" << std::endl;
				break;
			}
		}
		else
		{
			std::cout << "< Unknown command" << std::endl;
			continue;
		}
	loop_while:
	}
	return 0;
}
