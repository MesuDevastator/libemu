#include "mos6502_disassembler.h"

#include <map>

#include "mos6502_operation.h"

namespace
{
	using op = libemu::dasm::mos6502_operation;
	std::map<libemu::byte, op> operation_table
			{
					// adc
					{ 0x69, { op::adc,  0, op::immediate,   2, 2 }},
					{ 0x65, { op::adc,  0, op::zero_page,   2, 3 }},
					{ 0x75, { op::adc,  0, op::zero_page_x, 2, 4 }},
					{ 0x6d, { op::adc,  0, op::absolute,    3, 4 }},
					{ 0x7d, { op::adc,  0, op::absolute_x,  3, 4 }},
					{ 0x79, { op::adc,  0, op::absolute_y,  3, 4 }},
					{ 0x61, { op::adc,  0, op::indirect_x,  2, 6 }},
					{ 0x71, { op::adc,  0, op::indirect_y,  2, 5 }},
					// and
					{ 0x29, { op::and_, 0, op::immediate,   2, 2 }},
					{ 0x25, { op::and_, 0, op::zero_page,   2, 3 }},
					{ 0x35, { op::and_, 0, op::zero_page_x, 2, 4 }},
					{ 0x2d, { op::and_, 0, op::absolute,    3, 4 }},
					{ 0x3d, { op::and_, 0, op::absolute_x,  3, 4 }},
					{ 0x39, { op::and_, 0, op::absolute_y,  3, 4 }},
					{ 0x21, { op::and_, 0, op::indirect_x,  2, 6 }},
					{ 0x31, { op::and_, 0, op::indirect_y,  2, 5 }},
					// asl
					{ 0x0a, { op::asl,  0, op::accumulator, 1, 2 }},
					{ 0x06, { op::asl,  0, op::zero_page,   2, 5 }},
					{ 0x16, { op::asl,  0, op::zero_page_x, 2, 6 }},
					{ 0x0e, { op::asl,  0, op::absolute,    3, 6 }},
					{ 0x1e, { op::asl,  0, op::absolute_x,  3, 7 }},
					// bit
					{ 0x24, { op::bit,  0, op::zero_page,   2, 3 }},
					{ 0x2c, { op::bit,  0, op::absolute,    3, 4 }},
					// bpl
					{ 0x10, { op::bpl,  0, op::relative,    2, 2 }},
					// bmi
					{ 0x30, { op::bmi,  0, op::relative,    2, 2 }},
					// bvc
					{ 0x50, { op::bvc,  0, op::relative,    2, 2 }},
					// bvs
					{ 0x70, { op::bvs,  0, op::relative,    2, 2 }},
					// bcc
					{ 0x90, { op::bcc,  0, op::relative,    2, 2 }},
					// bcs
					{ 0xb0, { op::bcs,  0, op::relative,    2, 2 }},
					// bne
					{ 0xd0, { op::bne,  0, op::relative,    2, 2 }},
					// beq
					{ 0xf0, { op::beq,  0, op::relative,    2, 2 }},
					// brk
					{ 0x00, { op::brk,  0, op::implied,     2, 2 }},
					// cmp
					{ 0xc9, { op::cmp,  0, op::immediate,   2, 2 }},
					{ 0xc5, { op::cmp,  0, op::zero_page,   2, 3 }},
					{ 0xd5, { op::cmp,  0, op::zero_page_x, 2, 4 }},
					{ 0xcd, { op::cmp,  0, op::absolute,    3, 4 }},
					{ 0xdd, { op::cmp,  0, op::absolute_x,  3, 4 }},
					{ 0xd9, { op::cmp,  0, op::absolute_y,  3, 4 }},
					{ 0xc1, { op::cmp,  0, op::indirect_x,  2, 6 }},
					{ 0xd1, { op::cmp,  0, op::indirect_y,  2, 5 }},
					// cpx
					{ 0xe0, { op::cpx,  0, op::immediate,   2, 2 }},
					{ 0xe4, { op::cpx,  0, op::zero_page,   2, 3 }},
					{ 0xec, { op::cpx,  0, op::absolute,    3, 4 }},
					// cpy
					{ 0xc0, { op::cpy,  0, op::immediate,   2, 2 }},
					{ 0xc4, { op::cpy,  0, op::zero_page,   2, 3 }},
					{ 0xcc, { op::cpy,  0, op::absolute,    3, 4 }},
					// dec
					{ 0xc6, { op::dec,  0, op::zero_page,   2, 5 }},
					{ 0xd6, { op::dec,  0, op::zero_page_x, 2, 6 }},
					{ 0xce, { op::dec,  0, op::absolute,    3, 6 }},
					{ 0xde, { op::dec,  0, op::absolute_x,  3, 7 }},
					// eor
					{ 0x49, { op::eor,  0, op::immediate,   2, 2 }},
					{ 0x45, { op::eor,  0, op::zero_page,   2, 3 }},
					{ 0x55, { op::eor,  0, op::zero_page_x, 2, 4 }},
					{ 0x4d, { op::eor,  0, op::absolute,    3, 4 }},
					{ 0x5d, { op::eor,  0, op::absolute_x,  3, 4 }},
					{ 0x59, { op::eor,  0, op::absolute_y,  3, 4 }},
					{ 0x41, { op::eor,  0, op::indirect_x,  2, 6 }},
					{ 0x51, { op::eor,  0, op::indirect_y,  2, 5 }},
					// clc
					{ 0x18, { op::clc,  0, op::implied,     1, 2 }},
					// sec
					{ 0x38, { op::sec,  0, op::implied,     1, 2 }},
					// cli
					{ 0x58, { op::cli,  0, op::implied,     1, 2 }},
					// sei
					{ 0x78, { op::sei,  0, op::implied,     1, 2 }},
					// clv
					{ 0xb8, { op::clv,  0, op::implied,     1, 2 }},
					// cld
					{ 0xd8, { op::cld,  0, op::implied,     1, 2 }},
					// sed
					{ 0xf8, { op::sed,  0, op::implied,     1, 2 }},
					// inc
					{ 0xe6, { op::inc,  0, op::zero_page,   2, 5 }},
					{ 0xf6, { op::inc,  0, op::zero_page_x, 2, 6 }},
					{ 0xee, { op::inc,  0, op::absolute,    3, 6 }},
					{ 0xfe, { op::inc,  0, op::absolute_x,  3, 7 }},
					// jmp
					{ 0x4c, { op::jmp,  0, op::absolute,    3, 3 }},
					{ 0x6c, { op::jmp,  0, op::indirect,    3, 5 }},
					// jsr
					{ 0x20, { op::jsr,  0, op::absolute,    3, 6 }},
					// lda
					{ 0xa9, { op::lda,  0, op::immediate,   2, 2 }},
					{ 0xa5, { op::lda,  0, op::zero_page,   2, 3 }},
					{ 0xb5, { op::lda,  0, op::zero_page_x, 2, 4 }},
					{ 0xad, { op::lda,  0, op::absolute,    3, 4 }},
					{ 0xbd, { op::lda,  0, op::absolute_x,  3, 4 }},
					{ 0xb9, { op::lda,  0, op::absolute_y,  3, 4 }},
					{ 0xa1, { op::lda,  0, op::indirect_x,  2, 6 }},
					{ 0xb1, { op::lda,  0, op::indirect_y,  2, 5 }},
					// ldx
					{ 0xa2, { op::ldx,  0, op::immediate,   2, 2 }},
					{ 0xa6, { op::ldx,  0, op::zero_page,   2, 3 }},
					{ 0xb6, { op::ldx,  0, op::zero_page_y, 2, 4 }},
					{ 0xae, { op::ldx,  0, op::absolute,    3, 4 }},
					{ 0xbe, { op::ldx,  0, op::absolute_y,  3, 4 }},
					// ldy
					{ 0xa0, { op::ldy,  0, op::immediate,   2, 2 }},
					{ 0xa4, { op::ldy,  0, op::zero_page,   2, 3 }},
					{ 0xb4, { op::ldy,  0, op::zero_page_x, 2, 4 }},
					{ 0xac, { op::ldy,  0, op::absolute,    3, 4 }},
					{ 0xbc, { op::ldy,  0, op::absolute_x,  3, 4 }},
					// lsr
					{ 0x4a, { op::lsr,  0, op::accumulator, 1, 2 }},
					{ 0x46, { op::lsr,  0, op::zero_page,   2, 5 }},
					{ 0x56, { op::lsr,  0, op::zero_page_x, 2, 6 }},
					{ 0x4e, { op::lsr,  0, op::absolute,    3, 6 }},
					{ 0x5e, { op::lsr,  0, op::absolute_x,  3, 7 }},
					// nop
					{ 0xea, { op::nop,  0, op::implied,     1, 2 }},
					{ 0x1a, { op::nop,  0, op::implied,     1, 2 }},
					{ 0x3a, { op::nop,  0, op::implied,     1, 2 }},
					{ 0x5a, { op::nop,  0, op::implied,     1, 2 }},
					{ 0x7a, { op::nop,  0, op::implied,     1, 2 }},
					{ 0xda, { op::nop,  0, op::implied,     1, 2 }},
					{ 0xfa, { op::nop,  0, op::implied,     1, 2 }},
					{ 0x80, { op::nop,  0, op::immediate,   2, 2 }},
					{ 0x82, { op::nop,  0, op::immediate,   2, 2 }},
					{ 0x89, { op::nop,  0, op::immediate,   2, 2 }},
					{ 0xc2, { op::nop,  0, op::immediate,   2, 2 }},
					{ 0xe2, { op::nop,  0, op::immediate,   2, 2 }},
					{ 0x04, { op::nop,  0, op::zero_page,   2, 3 }},
					{ 0x44, { op::nop,  0, op::zero_page,   2, 3 }},
					{ 0x64, { op::nop,  0, op::zero_page,   2, 3 }},
					{ 0x14, { op::nop,  0, op::zero_page_x, 2, 4 }},
					{ 0x34, { op::nop,  0, op::zero_page_x, 2, 4 }},
					{ 0x54, { op::nop,  0, op::zero_page_x, 2, 4 }},
					{ 0x74, { op::nop,  0, op::zero_page_x, 2, 4 }},
					{ 0xd4, { op::nop,  0, op::zero_page_x, 2, 4 }},
					{ 0xf4, { op::nop,  0, op::zero_page_x, 2, 4 }},
					{ 0x0c, { op::nop,  0, op::absolute,    3, 4 }},
					{ 0x1c, { op::nop,  0, op::absolute_x,  3, 4 }},
					{ 0x3c, { op::nop,  0, op::absolute_x,  3, 4 }},
					{ 0x5c, { op::nop,  0, op::absolute_x,  3, 4 }},
					{ 0x7c, { op::nop,  0, op::absolute_x,  3, 4 }},
					{ 0xdc, { op::nop,  0, op::absolute_x,  3, 4 }},
					{ 0xfc, { op::nop,  0, op::absolute_x,  3, 4 }},
					// ora
					{ 0x09, { op::ora,  0, op::immediate,   2, 2 }},
					{ 0x05, { op::ora,  0, op::zero_page,   2, 3 }},
					{ 0x15, { op::ora,  0, op::zero_page_x, 2, 4 }},
					{ 0x0d, { op::ora,  0, op::absolute,    3, 4 }},
					{ 0x1d, { op::ora,  0, op::absolute_x,  3, 4 }},
					{ 0x19, { op::ora,  0, op::absolute_y,  3, 4 }},
					{ 0x01, { op::ora,  0, op::indirect_x,  2, 6 }},
					{ 0x11, { op::ora,  0, op::indirect_y,  2, 5 }},
					// tax
					{ 0xaa, { op::tax,  0, op::implied,     1, 2 }},
					// txa
					{ 0x8a, { op::txa,  0, op::implied,     1, 2 }},
					// dex
					{ 0xca, { op::dex,  0, op::implied,     1, 2 }},
					// inx
					{ 0xe8, { op::inx,  0, op::implied,     1, 2 }},
					// tay
					{ 0xa8, { op::tay,  0, op::implied,     1, 2 }},
					// tya
					{ 0x98, { op::tya,  0, op::implied,     1, 2 }},
					// dey
					{ 0x88, { op::dey,  0, op::implied,     1, 2 }},
					// iny
					{ 0xc8, { op::iny,  0, op::implied,     1, 2 }},
					// rol
					{ 0x2a, { op::rol,  0, op::accumulator, 1, 2 }},
					{ 0x26, { op::rol,  0, op::zero_page,   2, 5 }},
					{ 0x36, { op::rol,  0, op::zero_page_x, 2, 6 }},
					{ 0x2e, { op::rol,  0, op::absolute,    3, 6 }},
					{ 0x3e, { op::rol,  0, op::absolute_x,  3, 7 }},
					// ror
					{ 0x6a, { op::ror,  0, op::accumulator, 1, 2 }},
					{ 0x66, { op::ror,  0, op::zero_page,   2, 5 }},
					{ 0x76, { op::ror,  0, op::zero_page_x, 2, 6 }},
					{ 0x6e, { op::ror,  0, op::absolute,    3, 6 }},
					{ 0x7e, { op::ror,  0, op::absolute_x,  3, 7 }},
					// rti
					{ 0x40, { op::rti,  0, op::implied,     1, 6 }},
					// rts
					{ 0x60, { op::rts,  0, op::implied,     1, 6 }},
					// sbc
					{ 0xe9, { op::sbc,  0, op::immediate,   2, 2 }},
					{ 0xe5, { op::sbc,  0, op::zero_page,   2, 3 }},
					{ 0xf5, { op::sbc,  0, op::zero_page_x, 2, 4 }},
					{ 0xed, { op::sbc,  0, op::absolute,    3, 4 }},
					{ 0xfd, { op::sbc,  0, op::absolute_x,  3, 4 }},
					{ 0xf9, { op::sbc,  0, op::absolute_y,  3, 4 }},
					{ 0xe1, { op::sbc,  0, op::indirect_x,  2, 6 }},
					{ 0xf1, { op::sbc,  0, op::indirect_y,  2, 5 }},
					// sta
					{ 0x85, { op::sta,  0, op::zero_page,   2, 3 }},
					{ 0x95, { op::sta,  0, op::zero_page_x, 2, 4 }},
					{ 0x8d, { op::sta,  0, op::absolute,    3, 4 }},
					{ 0x9d, { op::sta,  0, op::absolute_x,  3, 5 }},
					{ 0x99, { op::sta,  0, op::absolute_y,  3, 5 }},
					{ 0x81, { op::sta,  0, op::indirect_x,  2, 6 }},
					{ 0x91, { op::sta,  0, op::indirect_y,  2, 6 }},
					// txs
					{ 0x9a, { op::txs,  0, op::implied,     1, 2 }},
					// tsx
					{ 0xba, { op::tsx,  0, op::implied,     1, 2 }},
					// pha
					{ 0x48, { op::pha,  0, op::implied,     1, 3 }},
					// pla
					{ 0x68, { op::pla,  0, op::implied,     1, 4 }},
					// php
					{ 0x08, { op::php,  0, op::implied,     1, 3 }},
					// plp
					{ 0x28, { op::plp,  0, op::implied,     1, 4 }},
					// stx
					{ 0x86, { op::stx,  0, op::zero_page,   2, 3 }},
					{ 0x96, { op::stx,  0, op::zero_page_y, 2, 4 }},
					{ 0x8e, { op::stx,  0, op::absolute,    3, 4 }},
					// sty
					{ 0x84, { op::sty,  0, op::zero_page,   2, 3 }},
					{ 0x94, { op::sty,  0, op::zero_page_y, 2, 4 }},
					{ 0x8c, { op::sty,  0, op::absolute,    3, 4 }},
					// alr
					{ 0x4b, { op::alr,  0, op::immediate,   2, 2 }},
					// anc
					{ 0x0b, { op::anc,  0, op::immediate,   2, 2 }},
					// anc2
					{ 0x2b, { op::anc2, 0, op::immediate,   2, 2 }},
					// ane
					{ 0x8b, { op::ane,  0, op::immediate,   2, 2 }},
					// arr
					{ 0x6b, { op::arr,  0, op::immediate,   2, 2 }},
					// dcp
					{ 0xc7, { op::dcp,  0, op::zero_page,   2, 5 }},
					{ 0xd7, { op::dcp,  0, op::zero_page_x, 2, 6 }},
					{ 0xcf, { op::dcp,  0, op::absolute,    3, 6 }},
					{ 0xdf, { op::dcp,  0, op::absolute_x,  3, 7 }},
					{ 0xdb, { op::dcp,  0, op::absolute_y,  3, 7 }},
					{ 0xc3, { op::dcp,  0, op::indirect_x,  2, 8 }},
					{ 0xd3, { op::dcp,  0, op::indirect_y,  2, 8 }},
					// isc
					{ 0xe7, { op::isc,  0, op::zero_page,   2, 5 }},
					{ 0xf7, { op::isc,  0, op::zero_page_x, 2, 6 }},
					{ 0xef, { op::isc,  0, op::absolute,    3, 6 }},
					{ 0xff, { op::isc,  0, op::absolute_x,  3, 7 }},
					{ 0xfb, { op::isc,  0, op::absolute_y,  3, 7 }},
					{ 0xe3, { op::isc,  0, op::indirect_x,  2, 8 }},
					{ 0xf3, { op::isc,  0, op::indirect_y,  2, 8 }},
					// las
					{ 0xbb, { op::las,  0, op::absolute_y,  3, 4 }},
					// lax
					{ 0xa7, { op::lax,  0, op::zero_page,   2, 3 }},
					{ 0xb7, { op::lax,  0, op::zero_page_x, 2, 4 }},
					{ 0xaf, { op::lax,  0, op::absolute,    3, 4 }},
					{ 0xbf, { op::lax,  0, op::absolute_y,  3, 4 }},
					{ 0xa3, { op::lax,  0, op::indirect_x,  2, 6 }},
					{ 0xb3, { op::lax,  0, op::indirect_y,  2, 5 }},
					// lxa
					{ 0xab, { op::lxa,  0, op::immediate,   2, 2 }},
					// rla
					{ 0x27, { op::rla,  0, op::zero_page,   2, 5 }},
					{ 0x37, { op::rla,  0, op::zero_page_x, 2, 6 }},
					{ 0x2f, { op::rla,  0, op::absolute,    3, 6 }},
					{ 0x3f, { op::rla,  0, op::absolute_x,  3, 7 }},
					{ 0x3b, { op::rla,  0, op::absolute_y,  3, 7 }},
					{ 0x23, { op::rla,  0, op::indirect_x,  2, 8 }},
					{ 0x33, { op::rla,  0, op::indirect_y,  2, 8 }},
					// rra
					{ 0x67, { op::rra,  0, op::zero_page,   2, 5 }},
					{ 0x77, { op::rra,  0, op::zero_page_x, 2, 6 }},
					{ 0x6f, { op::rra,  0, op::absolute,    3, 6 }},
					{ 0x7f, { op::rra,  0, op::absolute_x,  3, 7 }},
					{ 0x7b, { op::rra,  0, op::absolute_y,  3, 7 }},
					{ 0x63, { op::rra,  0, op::indirect_x,  2, 8 }},
					{ 0x73, { op::rra,  0, op::indirect_y,  2, 8 }},
					// sax
					{ 0x87, { op::sax,  0, op::zero_page,   2, 3 }},
					{ 0x97, { op::sax,  0, op::zero_page_x, 2, 4 }},
					{ 0x8f, { op::sax,  0, op::absolute,    3, 4 }},
					{ 0x83, { op::sax,  0, op::indirect_x,  2, 6 }},
					// sbx
					{ 0xcb, { op::sbx,  0, op::immediate,   2, 2 }},
					// sha
					{ 0x9f, { op::sha,  0, op::absolute_y,  3, 5 }},
					{ 0x93, { op::sha,  0, op::indirect_y,  2, 6 }},
					// shx
					{ 0x9e, { op::shx,  0, op::absolute_y,  3, 5 }},
					// shy
					{ 0x9c, { op::shy,  0, op::absolute_x,  3, 5 }},
					// slo
					{ 0x07, { op::slo,  0, op::zero_page,   2, 5 }},
					{ 0x17, { op::slo,  0, op::zero_page_x, 2, 6 }},
					{ 0x0f, { op::slo,  0, op::absolute,    3, 6 }},
					{ 0x1f, { op::slo,  0, op::absolute_x,  3, 7 }},
					{ 0x1b, { op::slo,  0, op::absolute_y,  3, 7 }},
					{ 0x03, { op::slo,  0, op::indirect_x,  2, 8 }},
					{ 0x13, { op::slo,  0, op::indirect_y,  2, 8 }},
					// sre
					{ 0x47, { op::sre,  0, op::zero_page,   2, 5 }},
					{ 0x57, { op::sre,  0, op::zero_page_x, 2, 6 }},
					{ 0x4f, { op::sre,  0, op::absolute,    3, 6 }},
					{ 0x5f, { op::sre,  0, op::absolute_x,  3, 7 }},
					{ 0x5b, { op::sre,  0, op::absolute_y,  3, 7 }},
					{ 0x43, { op::sre,  0, op::indirect_x,  2, 8 }},
					{ 0x53, { op::sre,  0, op::indirect_y,  2, 8 }},
					// tas
					{ 0x9b, { op::tas,  0, op::absolute_y,  3, 5 }},
					// usbc
					{ 0xeb, { op::usbc, 0, op::immediate,   2, 2 }},
					// jam
					{ 0x02, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x12, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x22, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x32, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x42, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x52, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x62, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x72, { op::jam,  0, op::implied,     1, 2 }},
					{ 0x92, { op::jam,  0, op::implied,     1, 2 }},
					{ 0xb2, { op::jam,  0, op::implied,     1, 2 }},
					{ 0xd2, { op::jam,  0, op::implied,     1, 2 }},
					{ 0xf2, { op::jam,  0, op::implied,     1, 2 }},
			};
}

namespace libemu::dasm
{
	mos6502_operation mos6502_disassembler::disassemble_internal(const byte* bytes, const offset begin_offset,
																 offset& end_offset) noexcept
	{
		mos6502_operation operation{ operation_table.at(bytes[0]) };
		for (unsigned long i{ 1 }; i < operation.instruction_length; ++i)
			operation.operator_ += (bytes[i] << ((i - 1) * 8));
		// Note: Timing correction
		// absolute_x:                      add 1 cycle if page boundary crossed
		//									except: asl, dec, inc, lsr, rol, ror, sta, dcp, isc, rla, rra, shy, slo, sre
		// absolute_y:                      add 1 cycle if page boundary crossed
		//									except: sta, dcp, isc, rla, rra, sha, shx, slo, sre, tas
		// indirect_y:                      add 1 cycle if page boundary crossed
		//									except: sta, dcp, isc, rla, rra, sha, slo, sre
		// relative (branch instructions):	add 1 cycle if branch succeeds
		// 									add 2 cycles if page boundary crossed
		return operation;
	}

	operation mos6502_disassembler::disassemble(const byte* bytes, offset begin_offset, offset& end_offset) noexcept
	{
		return operation{};
	}
}
