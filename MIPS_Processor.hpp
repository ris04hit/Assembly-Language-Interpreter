#ifndef __MIPS_PROCESSOR_HPP__
#define __MIPS_PROCESSOR_HPP__

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <unordered_set>
#include <algorithm>

struct MIPS_Architecture
{
	int registers[32] = {0}, PCcurr = 0, PCnext;
	std::unordered_map<std::string, std::function<int(int, int)>> function_map;
	std::unordered_map<std::string, int> registerMap, address;
	static const int MAX = (1 << 20);
	int data[MAX >> 2] = {0};
	std::unordered_map<int, int> memoryDelta;
	std::vector<std::vector<std::string>> commands, commands_stall;
	std::vector<int> commandCount;
	std::unordered_set<std::string> R_type, I_type, J_type, instructions;
	enum exit_code
	{
		SUCCESS = 0,
		INVALID_REGISTER,
		INVALID_LABEL,
		INVALID_ADDRESS,
		SYNTAX_ERROR,
		MEMORY_ERROR
	};
	struct Pipeline_5 {
		struct IF_ID {
			int PCcount = 0;
			std::vector<std::string> instruction = {"", "$0", "$0", "$0"};
		} if_id;
		struct ID_EX{
			int PCcount = 0, data1 = 0, data2 = 0;
			std::string instruction15_0 = "0", instruction20_16 = "$0", instruction15_11 = "$0", core_instruction = "", RegisterRS = "$0";
		} id_ex; 
		struct EX_MEM{
			int ALUresult = 0, data = 0;
			std::string instruction = "$0", core_instruction = "";
		} ex_mem;
		struct MEM_WB{
			int Readdata = 0, ALUresult = 0;
			std::string instruction = "$0", core_instruction = "";
		} mem_wb;
		struct Control {
			int RegDst = 0, Branch = 0, MemRead = 0, MemtoReg = 0, ALUOp0 = 0, ALUOp1 = 0, MemWrite = 0, ALUSrc = 0, RegWrite_wb = 0, RegWrite_mem = 0, PCsrc = 0;
		} control;
		void print(){
			// IF_ID
			std::cout << "if_id:\t" << "PCcount = " << if_id.PCcount <<"\tinstruction = "; 
			for (int i = 0; i<if_id.instruction.size(); i++){
				std::cout << if_id.instruction[i] << " ";
			}
			std::cout << "\n";
			// ID_EX
			std::cout << "id_ex:\t" << "PCcount = " << id_ex.PCcount << "\tdata1 = " << id_ex.data1 << "\tdata2 = " << id_ex.data2 << "\tinstruction15_0 = " << id_ex.instruction15_0 << "\tinstruction20_16 = " <<  id_ex.instruction20_16 << "\tinstruction15_11 = " << id_ex.instruction15_11 << "\tRegisterRS = " << id_ex.RegisterRS << "\tinst = " << id_ex.core_instruction << "\n";
			// EX_MEM
			std::cout << "ex_mem:\t" << "ALUresult = " << ex_mem.ALUresult << "\tdata = " << ex_mem.data << "\tinstruction = " << ex_mem.instruction << "\tRegDst = " << control.RegDst << "\tALUOp = " << control.ALUOp1 << control.ALUOp0 << "\tALUsrc = " << control.ALUSrc  << "\tBranch = " << control.Branch << "\tPCsrc = " << control.PCsrc << "\tinst = " << ex_mem.core_instruction << "\n";
			// MEM_WB
			std::cout << "mem_wb:\t" << "Readdata = " << mem_wb.Readdata << "\tALUresult = " << mem_wb.ALUresult << "\tinstruction = " << mem_wb.instruction << "\tMemRead = " << control.MemRead << "\tMemWrite = " << control.MemWrite << "\tRegWrite = " << control.RegWrite_mem << "\tinst = " << mem_wb.core_instruction << "\n";
			// End
		}
	};
	struct Pipeline_79 {
		struct IF1_IF2 {
			int PCcount = 0;
			std::vector<std::string> instruction = {"", "$0", "$0", "$0"};
		} if1_if2;
		struct IF2_ID1 {
			int PCcount = 0;
			std::vector<std::string> instruction = {"", "$0", "$0", "$0"};
		} if2_id1;
		struct ID1_ID2 {
			int PCcount = 0, immediate = 0;
			std::string core_instruction = "", RegisterRs = "$0", RegisterRt = "$0", RegisterRd = "$0";
		} id1_id2;
		struct ID2_RR {
			int PCcount = 0, immediate = 0;
			std::string core_instruction = "", RegisterRs = "$0", RegisterRt = "$0", RegisterRd = "$0";
		} id2_rr;
		struct RR_EX1 {
			int PCcount = 0, data1 = 0, data2 = 0;
			std::string core_instruction = "", RegisterStore = "$0", RegisterInp1 = "$0", RegisterInp2 = "$0";
		} rr_ex1;
		struct EX1_RW1 {
			int ALUresult = 0;
			std::string core_instruction = "", RegisterStore = "$0", Registerbuffer = "$0";
		} ex1_rw1;
		struct RR_EX2 {
			int data1 = 0, data2 = 0;
			std::string core_instruction = "", RegisterStore = "$0", RegisterInp = "$0";
		} rr_ex2;
		struct EX2_MEM1 {
			int address = 0;
			std::string core_instruction = "", RegisterStore = "$0";
		} ex2_mem1;
		struct MEM1_MEM2 {
			int address = 0;
			std::string core_instruction = "", RegisterStore = "$0";
		} mem1_mem2;
		struct MEM2_RW2 {
			int data = 0;
			std::string core_instruction = "", RegisterStore = "$0";
		} mem2_rw2;
		void print() {
			std::cout << "if1_if2:" << "\tPCcount = " << if1_if2.PCcount <<"\tinstruction = "; 
			for (int i = 0; i<if1_if2.instruction.size(); i++){
				std::cout << if1_if2.instruction[i] << " ";
			}
			std::cout << "\n";
			std::cout << "if2_id1:" << "\tPCcount = " << if2_id1.PCcount <<"\tinstruction = "; 
			for (int i = 0; i<if2_id1.instruction.size(); i++){
				std::cout << if2_id1.instruction[i] << " ";
			}
			std::cout << "\n";
			std::cout << "id1_id2:" << "\tPCcount = " << id1_id2.PCcount << "\timmediate = " << id1_id2.immediate << "\tcore_instruction = " << id1_id2.core_instruction << "\tRegisterRs = " << id1_id2.RegisterRs << "\tRegisterRt = " << id1_id2.RegisterRt << "\tRegisterRd = " << id1_id2.RegisterRd << "\n";
			std::cout << "id2_rr:\t" << "\tPCcount = " << id2_rr.PCcount << "\timmediate = " << id2_rr.immediate << "\tcore_instruction = " << id2_rr.core_instruction << "\tRegisterRs = " << id2_rr.RegisterRs << "\tRegisterRt = " << id2_rr.RegisterRt << "\tRegisterRd = " << id2_rr.RegisterRd << "\n";
			std::cout << "rr_ex1:\t" << "\tPCCount = " << rr_ex1.PCcount << "\tdata1 = " << rr_ex1.data1 << "\tdata2 = " << rr_ex1.data2 << "\tcore_instruction = " << rr_ex1.core_instruction << "\tRegisterStore = " << rr_ex1.RegisterStore << "\tRegisterInp1 = " << rr_ex1.RegisterInp1 << "\tRegisterInp2 = " << rr_ex1.RegisterInp2 << "\n";
			std::cout << "ex1_rw1:" << "\tALUresult = " << ex1_rw1.ALUresult << "\tcore_instruction = " << ex1_rw1.core_instruction << "\tRegisterStore = " << ex1_rw1.RegisterStore << "\tRegisterBuffer = " << ex1_rw1.Registerbuffer << "\n";
			std::cout << "rr_ex2:\t" << "\tdata1 = " << rr_ex2.data1 << "\tdata2 = " << rr_ex2.data2 << "\tcore_instruction = " << rr_ex2.core_instruction << "\tRegisterStore = " << rr_ex2.RegisterStore << "\tRegisterInp = " << rr_ex2.RegisterInp <<"\n";
			std::cout << "ex2_mem1:" << "\taddress = " << ex2_mem1.address << "\tcore_instruction = " << ex2_mem1.core_instruction << "\tRegisterStore = " << ex2_mem1.RegisterStore << "\n";
			std::cout << "mem1_mem2:" << "\taddress = " << mem1_mem2.address << "\tcore_instruction = " << mem1_mem2.core_instruction << "\tRegisterStore = " << mem1_mem2.RegisterStore << "\n";
			std::cout << "mem2_rw2:" << "\tdata = " << mem2_rw2.data << "\tcore_instruction = " << mem2_rw2.core_instruction << "\tRegisterStore = " << mem2_rw2.RegisterStore << "\n";
		}
	};

	// constructor to initialise the instruction set
	MIPS_Architecture(std::ifstream &file){
		function_map = {{"add", [&](int a, int b){return a+b;}}, {"sub", [&](int a, int b){return a-b;}}, {"mul", [&](int a, int b){return a*b;}}, {"addi", [&](int a, int b){return a+b;}}, {"and", [&](int a, int b){return a & b;}}, {"or", [&](int a, int b){return a | b;}}, {"", [&](int a, int b){return 0;}}};
		instructions = {"add", "sub", "mul", "beq", "bne", "slt", "j", "lw", "sw", "addi", "and", "or"};
		R_type = {"add", "sub", "mul", "slt", "sll", "srl", "and", "or"};
		I_type = {"beq", "bne", "lw", "sw", "addi"};
		J_type = {"j"};

		for (int i = 0; i < 32; ++i)
			registerMap["$" + std::to_string(i)] = i;
		registerMap["$zero"] = 0;
		registerMap["$at"] = 1;
		registerMap["$v0"] = 2;
		registerMap["$v1"] = 3;
		for (int i = 0; i < 4; ++i)
			registerMap["$a" + std::to_string(i)] = i + 4;
		for (int i = 0; i < 8; ++i)
			registerMap["$t" + std::to_string(i)] = i + 8, registerMap["$s" + std::to_string(i)] = i + 16;
		registerMap["$t8"] = 24;
		registerMap["$t9"] = 25;
		registerMap["$k0"] = 26;
		registerMap["$k1"] = 27;
		registerMap["$gp"] = 28;
		registerMap["$sp"] = 29;
		registerMap["$s8"] = 30;
		registerMap["$ra"] = 31;

		constructCommands(file);
		commandCount.assign(commands.size(), 0);
	}

	// checks if label is valid
	inline bool checkLabel(std::string str){
		return str.size() > 0 && isalpha(str[0]) && all_of(++str.begin(), str.end(), [](char c)
														   { return (bool)isalnum(c); }) &&
			   instructions.find(str) == instructions.end();
	}

	// checks if the register is a valid one
	inline bool checkRegister(std::string r){
		return registerMap.find(r) != registerMap.end();
	}

	// checks if all of the registers are valid or not
	bool checkRegisters(std::vector<std::string> regs){
		return std::all_of(regs.begin(), regs.end(), [&](std::string r)
						   { return checkRegister(r); });
	}

	/*
		handle all exit codes:
		0: correct execution
		1: register provided is incorrect
		2: invalid label
		3: unaligned or invalid address
		4: syntax error
		5: commands exceed memory limit
	*/
	void handleExit(exit_code code, int cycleCount){
		std::cout << '\n';
		switch (code)
		{
		case 1:
			std::cerr << "Invalid register provided or syntax error in providing register\n";
			break;
		case 2:
			std::cerr << "Label used not defined or defined too many times\n";
			break;
		case 3:
			std::cerr << "Unaligned or invalid memory address specified\n";
			break;
		case 4:
			std::cerr << "Syntax error encountered\n";
			break;
		case 5:
			std::cerr << "Memory limit exceeded\n";
			break;
		default:
			break;
		}
		if (code != 0)
		{
			std::cerr << "Error encountered at:\n";
			for (auto &s : commands_stall[PCcurr])
				std::cerr << s << ' ';
			std::cerr << '\n';
		}
	}

	// parse the command assuming correctly formatted MIPS instruction (or label)
	void parseCommand(std::string line){
		// strip until before the comment begins
		line = line.substr(0, line.find('#'));
		std::vector<std::string> command;
		boost::tokenizer<boost::char_separator<char>> tokens(line, boost::char_separator<char>(", \t"));
		for (auto &s : tokens)
			command.push_back(s);
		// empty line or a comment only line
		if (command.empty())
			return;
		else if (command.size() == 1)
		{
			std::string label = command[0].back() == ':' ? command[0].substr(0, command[0].size() - 1) : "?";
			if (address.find(label) == address.end())
				address[label] = commands.size();
			else
				address[label] = -1;
			command.clear();
		}
		else if (command[0].back() == ':')
		{
			std::string label = command[0].substr(0, command[0].size() - 1);
			if (address.find(label) == address.end())
				address[label] = commands.size();
			else
				address[label] = -1;
			command = std::vector<std::string>(command.begin() + 1, command.end());
		}
		else if (command[0].find(':') != std::string::npos)
		{
			int idx = command[0].find(':');
			std::string label = command[0].substr(0, idx);
			if (address.find(label) == address.end())
				address[label] = commands.size();
			else
				address[label] = -1;
			command[0] = command[0].substr(idx + 1);
		}
		else if (command[1][0] == ':')
		{
			if (address.find(command[0]) == address.end())
				address[command[0]] = commands.size();
			else
				address[command[0]] = -1;
			command[1] = command[1].substr(1);
			if (command[1] == "")
				command.erase(command.begin(), command.begin() + 2);
			else
				command.erase(command.begin(), command.begin() + 1);
		}
		if (command.empty())
			return;
		if (command.size() > 4)
			for (int i = 4; i < (int)command.size(); ++i)
				command[3] += " " + command[i];
		if (command.size() == 3){
			std::string location = command[2];
			if (location.back() == ')'){
				int lparen = location.find('(');
				std::string offset = lparen == 0 ? "0" : location.substr(0, lparen);
				std::string reg = location.substr(lparen + 1);
				reg.pop_back();
				command[2] = reg;
				command.push_back(offset);
			}
			else {
				command.push_back("0");
			}
		}
		command.resize(4);
		commands.push_back(command);
	}

	// construct the commands vector from the input file
	void constructCommands(std::ifstream &file){
		std::string line;
		while (getline(file, line))
			parseCommand(line);
		file.close();
	}

	// construct 5 stage pipeline
	void PipelineStage5(bool forward){
		int clockCycles = 0;
		Pipeline_5 pipeline;
		std::string ReadRegister1, ReadRegister2, instruction15_0, instruction20_16, instruction15_11, core_instruction_ex, core_instruction_mem, core_instruction_wb, ALU_operation, instruction, Write_register, RegisterRS, fwd_instruction_mem_ex, fwd_instruction_wb_ex, fwd_core_wb_ex, fwd_instruction_wb_mem, fwd_core_wb_mem; 
		std::vector<std::string> operation, command;
		int PCcount_if, PCcount_id, PCcount_ex, data1, data2, ALU_operand, ALU_result_mem, ALU_result_wb, Write_data, Read_data, command_inp = 0, command_out = 0, fwd_ALU_mem_ex, fwd_ALU_wb_ex, fwd_read_wb_ex, fwd_Readdata_wb_mem;

		// Initial state Print
		// std::cout << clockCycles << " " << PCnext << "\n";
		// pipeline.print();
		// std::cout << command_inp << " " << command_out << "\n";
		printRegistersAndMemoryDelta();
		// std::cout << "\n";

		while ((PCcurr < commands_stall.size()) || (command_inp > command_out) || (pipeline.if_id.instruction[0] != "")){
			++clockCycles;

		// First half
			// WB: WriteBack
			core_instruction_wb = pipeline.mem_wb.core_instruction;
			Read_data = pipeline.mem_wb.Readdata;
			Write_register = pipeline.mem_wb.instruction;
			ALU_result_wb = pipeline.mem_wb.ALUresult;
			pipeline.control.RegWrite_wb = R_type.count(core_instruction_wb) || (core_instruction_wb == "lw") || (core_instruction_wb == "addi") ? 1 : 0;
			pipeline.control.MemtoReg = (core_instruction_wb == "lw") ? 1 : 0;
			if (pipeline.control.RegWrite_wb){
				if ((!checkRegister(Write_register)) || (registerMap[Write_register] == 0)){
					handleExit(INVALID_REGISTER, clockCycles);
					return;
				}
				if (core_instruction_wb == "slt") ALU_result_wb = ALU_result_wb < 0 ? 1 : 0;
				registers[registerMap[Write_register]] = pipeline.control.MemtoReg ? Read_data : ALU_result_wb;
			}
			if (core_instruction_wb != ""){
				command_out += 1;
			}

			// MEM: Memory Access
			ALU_result_mem = pipeline.ex_mem.ALUresult;
			Write_data = pipeline.ex_mem.data;
			core_instruction_mem = pipeline.ex_mem.core_instruction;
			instruction = pipeline.ex_mem.instruction;
			pipeline.control.MemRead = core_instruction_mem == "lw" ? 1 : 0;
			pipeline.control.MemWrite = core_instruction_mem == "sw" ? 1 : 0;
			pipeline.control.RegWrite_mem = R_type.count(core_instruction_mem)||(core_instruction_mem == "lw")||(core_instruction_mem == "addi") ? 1 : 0;
			fwd_core_wb_mem = pipeline.mem_wb.core_instruction;
			fwd_instruction_wb_mem = pipeline.mem_wb.instruction;
			fwd_Readdata_wb_mem = pipeline.mem_wb.Readdata;

			// EX: Execute
			PCcount_ex = pipeline.id_ex.PCcount;
			data1 = pipeline.id_ex.data1;
			data2 = pipeline.id_ex.data2;
			instruction15_0 = pipeline.id_ex.instruction15_0;
			instruction15_11 = pipeline.id_ex.instruction15_11;
			instruction20_16 = pipeline.id_ex.instruction20_16;
			core_instruction_ex = pipeline.id_ex.core_instruction;
			RegisterRS = pipeline.id_ex.RegisterRS;
			pipeline.control.RegDst = (R_type.count(core_instruction_ex)) ? 1 : 0;
			pipeline.control.ALUOp0 = 0;
			pipeline.control.ALUOp1 = 0;
			if ((core_instruction_ex == "bne") || (core_instruction_ex == "beq") || (core_instruction_ex == "slt")){
				pipeline.control.ALUOp0 = 1;
			}
			else if (function_map.count(core_instruction_ex)){
				pipeline.control.ALUOp1 = 1;
			}
			pipeline.control.ALUSrc = (core_instruction_ex == "lw") || (core_instruction_ex == "sw") || (core_instruction_ex == "addi") ? 1 : 0;
			fwd_instruction_mem_ex = pipeline.ex_mem.instruction;
			fwd_instruction_wb_ex = pipeline.mem_wb.instruction;
			fwd_ALU_mem_ex = pipeline.ex_mem.ALUresult;
			fwd_ALU_wb_ex = pipeline.mem_wb.ALUresult;
			fwd_read_wb_ex = pipeline.mem_wb.Readdata;
			fwd_core_wb_ex = pipeline.mem_wb.core_instruction;

			// ID: Instruction Decode
				PCcount_id = pipeline.if_id.PCcount;
				operation = pipeline.if_id.instruction;
				if (I_type.count(operation[0])) {
					ReadRegister1 = operation[2];
					ReadRegister2 = operation[1];
				}
				else if (R_type.count(operation[0])) {
					ReadRegister1 = operation[2];
					ReadRegister2 = operation[3];
				}
				else{
					ReadRegister1 = "$0";
					ReadRegister2 = "$0";
				}

			// IF : Instruction Fetch
				PCcount_if = PCcurr + 1;
				if (PCcurr >= commands_stall.size()){
					command = {"", "$0", "$0", "$0"};
				}
				else {
					command = commands_stall[PCcurr];
				}
			 
		// Second half
			// WB: WriteBack

			// MEM: Memory Access
			if (pipeline.control.MemWrite){
				if (forward){
					if ((fwd_core_wb_mem == "lw") && (registerMap[fwd_instruction_wb_mem] != 0) && (registerMap[fwd_instruction_wb_mem] == registerMap[instruction])){
						Write_data = fwd_Readdata_wb_mem;
					}
				}
				if (data[ALU_result_mem] != Write_data) {
					memoryDelta[ALU_result_mem >> 2] = Write_data;
				}
				data[ALU_result_mem >> 2] = Write_data;
			}
			if (pipeline.control.MemRead) pipeline.mem_wb.Readdata = data[ALU_result_mem >> 2];
			pipeline.mem_wb.ALUresult = ALU_result_mem;
			pipeline.mem_wb.instruction = instruction;
			pipeline.mem_wb.core_instruction = core_instruction_mem;

			// Ex: Execute
			if ((core_instruction_ex == "beq") || (core_instruction_ex == "bne")){
				if (!checkLabel(instruction15_0)){
					handleExit(SYNTAX_ERROR, clockCycles);
					return;
				}
				if (address.find(instruction15_0) == address.end() || address[instruction15_0] == -1){
					handleExit(INVALID_LABEL, clockCycles);
					return;		
				}
				instruction15_0 = std::to_string(address[instruction15_0] - PCcount_ex);
			}
			if (forward){
				if (pipeline.control.RegWrite_mem && (registerMap[fwd_instruction_mem_ex] != 0) && (registerMap[fwd_instruction_mem_ex] == registerMap[RegisterRS])){
					data1 = fwd_ALU_mem_ex;
				}
				else if (pipeline.control.RegWrite_wb && (registerMap[fwd_instruction_wb_ex] != 0) && (registerMap[fwd_instruction_wb_ex] == registerMap[RegisterRS])){
					data1 = (fwd_core_wb_ex == "lw") ? fwd_read_wb_ex : fwd_ALU_wb_ex;
				}
				if (pipeline.control.RegWrite_mem && (registerMap[fwd_instruction_mem_ex] != 0) && (registerMap[fwd_instruction_mem_ex] == registerMap[instruction20_16])){
					data2 = fwd_ALU_mem_ex;
				}
				else if (pipeline.control.RegWrite_wb && (registerMap[fwd_instruction_wb_ex] != 0) && (registerMap[fwd_instruction_wb_ex] == registerMap[instruction20_16])){
					data2 = (fwd_core_wb_ex == "lw") ? fwd_read_wb_ex : fwd_ALU_wb_ex;
				}
			}
			try {
				PCcount_ex = PCcount_ex + stoi(instruction15_0);
				ALU_operand = pipeline.control.ALUSrc ? stoi(instruction15_0) : data2;
			}
			catch(std::exception &e) {
				handleExit(SYNTAX_ERROR, clockCycles);
				return;
			}
			if (pipeline.control.ALUOp0){
				ALU_operation = "sub";
			}
			else if (pipeline.control.ALUOp1){
				ALU_operation = core_instruction_ex;
			}
			else {
				ALU_operation = "add";
			}
			pipeline.ex_mem.ALUresult = function_map[ALU_operation](data1, ALU_operand);
			pipeline.ex_mem.data = data2;
			pipeline.ex_mem.instruction = pipeline.control.RegDst ? instruction15_11 : instruction20_16;
			pipeline.ex_mem.core_instruction = core_instruction_ex;
			pipeline.control.Branch = (core_instruction_ex == "beq") || (core_instruction_ex == "bne") ? 1 : 0;
			if (pipeline.control.Branch){
				if ((core_instruction_ex == "beq") && (pipeline.ex_mem.ALUresult==0)){
					pipeline.control.PCsrc = 1;
				}
				else if ((core_instruction_ex == "bne") && (pipeline.ex_mem.ALUresult==1)){
					pipeline.control.PCsrc = 1;
				}
				else {
					pipeline.control.PCsrc = 0;
				}
			}
			else {
				pipeline.control.PCsrc = 0;
			}


			// ID: Instruction Decode
			if (operation[0] != "") command_inp += 1;
			pipeline.id_ex.PCcount = PCcount_id;
			if (!checkRegisters({ReadRegister1, ReadRegister2})){
				handleExit(INVALID_REGISTER, clockCycles);
				return;
			}
			pipeline.id_ex.data1 = registers[registerMap[ReadRegister1]];
			pipeline.id_ex.data2 = registers[registerMap[ReadRegister2]];
			pipeline.id_ex.instruction15_0 = I_type.count(operation[0]) ? operation[3] : "0";
			pipeline.id_ex.instruction20_16 = ReadRegister2;
			pipeline.id_ex.instruction15_11 = R_type.count(operation[0]) ? operation[1] : "$0";
			pipeline.id_ex.core_instruction = operation[0];
			pipeline.id_ex.RegisterRS = ReadRegister1;

			// IF: Instruction Fetch
			pipeline.if_id.PCcount = PCcount_if;
			pipeline.if_id.instruction = command;
			if ((instructions.find(command[0]) == instructions.end()) && (command[0] != ""))
			{
				handleExit(SYNTAX_ERROR, clockCycles);
				return;
			}

			// jump statement
			if (PCcurr >= commands_stall.size()){
				PCnext = PCcurr;
			}
			else if (pipeline.control.PCsrc){
				PCnext = PCcount_ex;
			}
			else if (operation[0] == "j"){
				if (!checkLabel(operation[1])){
						handleExit(SYNTAX_ERROR, clockCycles);
						return;
				}
				if (address.find(operation[1]) == address.end() || address[operation[1]] == -1){
						handleExit(INVALID_LABEL, clockCycles);
						return;		
				}
				PCnext = address[operation[1]];
			}
			else {
				PCnext = PCcount_if;
			}

		// Print
			// std::cout << clockCycles << " " << PCnext << "\n";
			// pipeline.print();
			// std::cout << command_inp << " " << command_out << "\n";
			printRegistersAndMemoryDelta();
			// std::cout << "\n";

		// Incrementing iteration
			PCcurr = PCnext;
		}
		handleExit(SUCCESS, clockCycles);
	}

	// construct 9 stage pipeline
	void PipelineStage9 (bool forward){
		int clockCycles = 0;
		Pipeline_79 pipeline;
		std::vector<std::string> command;
		int command_inp = 0, command_out = 0, RegWrite_rw1 = 0, RegWrite_rw2 = 0, data_rw2;
		std::string RegisterStore_rw = "$0";
		bool condition;

		// Initial state Print
		// std::cout << clockCycles << " " << PCnext << "\n";
		// pipeline.print();
		// std::cout << command_inp << " " << command_out << "\n";
		printRegistersAndMemoryDelta();
		// std::cout << "\n";

		while ((PCcurr < commands_stall.size()) || (command_inp > command_out) || (pipeline.if1_if2.instruction[0] != "")){
			++clockCycles ;

		// Full Cycle
			// RW2: Register Write 2
			if ((pipeline.mem2_rw2.core_instruction == "lw") && ((registerMap[pipeline.ex1_rw1.Registerbuffer] == 0) || registerMap[pipeline.ex1_rw1.Registerbuffer] != registerMap[pipeline.mem2_rw2.RegisterStore])){
				registers[registerMap[pipeline.mem2_rw2.RegisterStore]] = pipeline.mem2_rw2.data;
			}
			if (pipeline.mem2_rw2.core_instruction != "") command_out += 1;
			RegWrite_rw2 = (pipeline.mem2_rw2.core_instruction == "lw") ? 1 : 0;
			RegisterStore_rw = pipeline.mem2_rw2.RegisterStore;
			data_rw2 = pipeline.mem2_rw2.data;

			// MEM2
			pipeline.mem2_rw2.core_instruction = pipeline.mem1_mem2.core_instruction;
			pipeline.mem2_rw2.data = data[pipeline.mem1_mem2.address >> 2];
			pipeline.mem2_rw2.RegisterStore = pipeline.mem1_mem2.RegisterStore;
			if (pipeline.mem1_mem2.core_instruction == "sw"){
				if (data[pipeline.mem1_mem2.address >> 2] != registers[registerMap[pipeline.mem1_mem2.RegisterStore]]){
					memoryDelta[pipeline.mem1_mem2.address >> 2] = registers[registerMap[pipeline.mem1_mem2.RegisterStore]];
				}
				data[pipeline.mem1_mem2.address >> 2] = registers[registerMap[pipeline.mem1_mem2.RegisterStore]];
			}

			// MEM1
			pipeline.mem1_mem2.address = pipeline.ex2_mem1.address;
			pipeline.mem1_mem2.core_instruction = pipeline.ex2_mem1.core_instruction;
			pipeline.mem1_mem2.RegisterStore = pipeline.ex2_mem1.RegisterStore;

			// EX2
			if (forward){
				RegWrite_rw1 = ((pipeline.ex1_rw1.core_instruction == "addi") || (R_type.count(pipeline.ex1_rw1.core_instruction))) ? 1 : 0;
				if (RegWrite_rw1 && (registerMap[pipeline.ex1_rw1.RegisterStore] != 0) && (registerMap[pipeline.ex1_rw1.RegisterStore] == registerMap[pipeline.rr_ex2.RegisterInp])){
					pipeline.rr_ex2.data1 = pipeline.ex1_rw1.ALUresult;
				}
				else if (RegWrite_rw2 && (registerMap[RegisterStore_rw] != 0) && (registerMap[RegisterStore_rw] == registerMap[pipeline.rr_ex2.RegisterInp])){
					pipeline.rr_ex2.data1 = data_rw2;
				}
			}
			pipeline.ex2_mem1.address = function_map["add"](pipeline.rr_ex2.data1, pipeline.rr_ex2.data2);
			pipeline.ex2_mem1.core_instruction = pipeline.rr_ex2.core_instruction;
			pipeline.ex2_mem1.RegisterStore = pipeline.rr_ex2.RegisterStore;

			// RW1: Register Write 1
			if ((pipeline.ex1_rw1.core_instruction == "addi") || (R_type.count(pipeline.ex1_rw1.core_instruction))){
				registers[registerMap[pipeline.ex1_rw1.RegisterStore]] = pipeline.ex1_rw1.ALUresult;
				pipeline.ex1_rw1.Registerbuffer = pipeline.ex1_rw1.RegisterStore;
			}
			else {
				pipeline.ex1_rw1.Registerbuffer = "$0";
			}
			if (pipeline.ex1_rw1.core_instruction != "") command_out += 1;

			// EX1
			if (forward){
				RegWrite_rw1 = ((pipeline.ex1_rw1.core_instruction == "addi") || (R_type.count(pipeline.ex1_rw1.core_instruction))) ? 1 : 0;
				if (RegWrite_rw1 && (registerMap[pipeline.ex1_rw1.RegisterStore] != 0) && (registerMap[pipeline.ex1_rw1.RegisterStore] == registerMap[pipeline.rr_ex1.RegisterInp1])){
					pipeline.rr_ex1.data1 = pipeline.ex1_rw1.ALUresult;
				}
				else if (RegWrite_rw2 && (registerMap[RegisterStore_rw] != 0) && (registerMap[RegisterStore_rw] == registerMap[pipeline.rr_ex1.RegisterInp1])){
					pipeline.rr_ex1.data1 = data_rw2;
				}
				if (R_type.count(pipeline.rr_ex1.core_instruction)){
					if (RegWrite_rw1 && (registerMap[pipeline.ex1_rw1.RegisterStore] != 0) && (registerMap[pipeline.ex1_rw1.RegisterStore] == registerMap[pipeline.rr_ex1.RegisterInp2])){
						pipeline.rr_ex1.data2 = pipeline.ex1_rw1.ALUresult;
					}
					else if (RegWrite_rw2 && (registerMap[RegisterStore_rw] != 0) && (registerMap[RegisterStore_rw] == registerMap[pipeline.rr_ex1.RegisterInp2])){
						pipeline.rr_ex1.data2 = data_rw2;
					}
				}
			}
			pipeline.ex1_rw1.core_instruction = pipeline.rr_ex1.core_instruction;
			pipeline.ex1_rw1.RegisterStore = pipeline.rr_ex1.RegisterStore;
			if ((pipeline.rr_ex1.core_instruction == "beq") || (pipeline.rr_ex1.core_instruction == "bne")){
				pipeline.ex1_rw1.ALUresult = function_map["sub"](pipeline.rr_ex1.data1, registers[registerMap[pipeline.rr_ex1.RegisterStore]]);
			}
			else if (pipeline.rr_ex1.core_instruction == "slt"){
				pipeline.ex1_rw1.ALUresult = function_map["sub"](pipeline.rr_ex1.data1, pipeline.rr_ex1.data2) < 0 ? 1 : 0;
			}
			else if (pipeline.rr_ex1.core_instruction == "j"){
				pipeline.ex1_rw1.ALUresult = 0;
			}
			else {
				pipeline.ex1_rw1.ALUresult = function_map[pipeline.rr_ex1.core_instruction](pipeline.rr_ex1.data1, pipeline.rr_ex1.data2);
			}
				// Changing PCnext
			if ((pipeline.rr_ex1.core_instruction == "beq") || (pipeline.rr_ex1.core_instruction == "bne")){
				condition = ((pipeline.rr_ex1.core_instruction == "beq") && (pipeline.ex1_rw1.ALUresult == 0)) || ((pipeline.rr_ex1.core_instruction == "bne") && (pipeline.ex1_rw1.ALUresult != 0));
				PCnext = condition ? pipeline.rr_ex1.data2 : PCcurr + 1;
			}
			else {
				PCnext = PCcurr + 1;
			}

			// RR : Read Register
			if (!checkRegisters({pipeline.id2_rr.RegisterRs, pipeline.id2_rr.RegisterRd, pipeline.id2_rr.RegisterRt})){
				handleExit(INVALID_REGISTER, clockCycles);
				return;
			}
			if ((pipeline.id2_rr.core_instruction == "lw") || (pipeline.id2_rr.core_instruction == "sw")){
				pipeline.rr_ex1.data1 = 0;
				pipeline.rr_ex1.data2 = 0;
				pipeline.rr_ex1.RegisterStore = "$0";
				pipeline.rr_ex1.PCcount = 0;
				pipeline.rr_ex1.core_instruction = "";
				pipeline.rr_ex1.RegisterInp1 = "$0";
				pipeline.rr_ex1.RegisterInp2 = "$0";
				pipeline.rr_ex2.core_instruction = pipeline.id2_rr.core_instruction;
				pipeline.rr_ex2.data1 = registers[registerMap[pipeline.id2_rr.RegisterRs]];
				pipeline.rr_ex2.data2 = pipeline.id2_rr.immediate;
				pipeline.rr_ex2.RegisterStore = pipeline.id2_rr.RegisterRt;
				pipeline.rr_ex2.RegisterInp = pipeline.id2_rr.RegisterRs;
			}
			else {
				pipeline.rr_ex2.core_instruction = "";
				pipeline.rr_ex2.data1 = 0;
				pipeline.rr_ex2.data2 = 0;
				pipeline.rr_ex2.RegisterStore = "$0";
				pipeline.rr_ex2.RegisterInp = "$0";
				pipeline.rr_ex1.PCcount = pipeline.id2_rr.PCcount;
				pipeline.rr_ex1.core_instruction = pipeline.id2_rr.core_instruction;
				pipeline.rr_ex1.data1 = registers[registerMap[pipeline.id2_rr.RegisterRs]];
				if (R_type.count(pipeline.id2_rr.core_instruction)){
					pipeline.rr_ex1.data2 = registers[registerMap[pipeline.id2_rr.RegisterRt]];
					pipeline.rr_ex1.RegisterStore = pipeline.id2_rr.RegisterRd;
					pipeline.rr_ex1.RegisterInp2 = pipeline.id2_rr.RegisterRt;
				}
				else {
					pipeline.rr_ex1.data2 = pipeline.id2_rr.immediate;
					pipeline.rr_ex1.RegisterStore = pipeline.id2_rr.RegisterRt;
					pipeline.rr_ex1.RegisterInp2 = "$0";
				}
				pipeline.rr_ex1.RegisterInp1 = pipeline.id2_rr.RegisterRs;
			}

			// ID2
			pipeline.id2_rr.core_instruction = pipeline.id1_id2.core_instruction;
			pipeline.id2_rr.immediate = pipeline.id1_id2.immediate;
			pipeline.id2_rr.PCcount = pipeline.id1_id2.PCcount;
			pipeline.id2_rr.RegisterRd = pipeline.id1_id2.RegisterRd;
			pipeline.id2_rr.RegisterRs = pipeline.id1_id2.RegisterRs;
			pipeline.id2_rr.RegisterRt = pipeline.id1_id2.RegisterRt;
			if (pipeline.id2_rr.core_instruction == "j") PCnext = pipeline.id2_rr.immediate;

			// ID1
			pipeline.id1_id2.PCcount = pipeline.if2_id1.PCcount;
			pipeline.id1_id2.core_instruction = pipeline.if2_id1.instruction[0];
			if (R_type.count(pipeline.if2_id1.instruction[0])){
				pipeline.id1_id2.RegisterRd = pipeline.if2_id1.instruction[1];
				pipeline.id1_id2.RegisterRs = pipeline.if2_id1.instruction[2];
				pipeline.id1_id2.RegisterRt = pipeline.if2_id1.instruction[3];
				pipeline.id1_id2.immediate = 0;
			}
			else if (I_type.count(pipeline.if2_id1.instruction[0])){
				pipeline.id1_id2.RegisterRd = "$0";
				pipeline.id1_id2.RegisterRt = pipeline.if2_id1.instruction[1];
				pipeline.id1_id2.RegisterRs = pipeline.if2_id1.instruction[2];
				if ((pipeline.if2_id1.instruction[0] == "beq") || (pipeline.if2_id1.instruction[0] == "bne")){
					if (checkLabel(pipeline.if2_id1.instruction[3])){
						pipeline.id1_id2.immediate = address[pipeline.if2_id1.instruction[3]];
					}
					else{
						handleExit(INVALID_LABEL, clockCycles);
						return;
					}
				}
				else {
					try {
						pipeline.id1_id2.immediate = stoi(pipeline.if2_id1.instruction[3]);
					}
					catch (std::exception &e){
						handleExit(SYNTAX_ERROR, clockCycles);
						return;
					}
				}
			}
			else if (J_type.count(pipeline.if2_id1.instruction[0])){
				pipeline.id1_id2.RegisterRd = "$0";
				pipeline.id1_id2.RegisterRt = "$0";
				pipeline.id1_id2.RegisterRs = "$0";
				if (checkLabel(pipeline.if2_id1.instruction[1])){
					pipeline.id1_id2.immediate = address[pipeline.if2_id1.instruction[1]];
				}
				else{
					handleExit(INVALID_LABEL, clockCycles);
					return;
				}
			}
			else {
				pipeline.id1_id2.RegisterRd = "$0";
				pipeline.id1_id2.RegisterRs = "$0";
				pipeline.id1_id2.RegisterRt = "$0";
				pipeline.id1_id2.immediate = 0;
			}

			// IF2
			pipeline.if2_id1.PCcount = pipeline.if1_if2.PCcount;
			pipeline.if2_id1.instruction = pipeline.if1_if2.instruction;

			// IF1
			if (PCcurr >= commands_stall.size()){
				pipeline.if1_if2.instruction = {"", "$0", "$0", "$0"};
			}
			else {
				pipeline.if1_if2.instruction = commands_stall[PCcurr];
			}
			pipeline.if1_if2.PCcount = PCcurr + 1;
			if (pipeline.if1_if2.instruction[0] != "") command_inp += 1;

		// Print
			// std::cout << clockCycles << " " << PCnext << "\n";
			// pipeline.print();
			// std::cout << command_inp << " " << command_out << "\n";
			printRegistersAndMemoryDelta();
			// std::cout << "\n";

		// Incrementing iteration
			PCcurr = PCnext;
		}
		handleExit(SUCCESS, clockCycles);
	}

	// execute the commands with pipelining, stage = true means 5 stage pipeline and stage = false means 7-9 stage pipeline
	void executeCommandsPipelined(bool forward, bool stage){
		if (commands.size() >= MAX / 4)
		{
			handleExit(MEMORY_ERROR, 0);
			return;
		}

		// Stalling
		std::vector<std::vector<std::string>> commands_pipe;
		int add_line = 0;

		// Stalls for beq, bne and j
		for (int i = 0; i<commands.size(); i++){
			add_line = 0;
			commands_pipe.push_back(commands[i]);
			if ((commands[i][0] == "beq") || (commands[i][0] == "bne")){
				add_line = stage ? 2 : 5;
				for (int i = 0; i < add_line; i++) commands_pipe.push_back({"","$0","$0","$0"});
			}
			else if (commands[i][0] == "j") {
				add_line = stage ? 1 : 3;
				for (int i = 0; i < add_line; i++) commands_pipe.push_back({"","$0","$0","$0"});
			}
			if (add_line){
				for (auto it = address.begin(); it != address.end(); ++it){
					if (it->second > commands_pipe.size()-1-add_line) it->second += add_line;
				}
			}
		}
		int register_wait[32] = {0};
		std::vector<std::string> command_pipe;
		bool condition;
		int wait_lw = 0;

		// Stalls for data hazard
		for (int i = 0; i< commands_pipe.size(); i++){
			add_line = 0;
			command_pipe = commands_pipe[i];
			if (R_type.count(command_pipe[0])){
				if (!checkRegisters({command_pipe[2],command_pipe[3]})){
					handleExit(INVALID_REGISTER, commands_stall.size());
					return;
				}
				add_line = std::max(register_wait[registerMap[command_pipe[2]]], register_wait[registerMap[command_pipe[3]]]);
			}
			else if ((command_pipe[0] == "sw") && wait_lw == registerMap[command_pipe[1]]){
				if (!checkRegisters({command_pipe[1],command_pipe[2]})){
					handleExit(INVALID_REGISTER, commands_stall.size());
					return;
				}
				add_line = (stage) ? register_wait[registerMap[command_pipe[2]]] : std::max(register_wait[registerMap[command_pipe[2]]], 1);
			}
			else if ((command_pipe[0] == "sw") || (command_pipe[0] == "beq" || (command_pipe[0] == "bne"))){
				if (!checkRegisters({command_pipe[1],command_pipe[2]})){
					handleExit(INVALID_REGISTER, commands_stall.size());
					return;
				}
				add_line = std::max(register_wait[registerMap[command_pipe[1]]], register_wait[registerMap[command_pipe[2]]]);
			}
			else if (I_type.count(command_pipe[0])){ 
				if (!checkRegister(command_pipe[2])){
					handleExit(INVALID_REGISTER, commands_stall.size());
					return;
				}
				add_line = register_wait[registerMap[command_pipe[2]]];
			}
			if (add_line) {
				for (int j = 0; j<add_line; j++) commands_stall.push_back({"","$0","$0","$0"});
			}
			commands_stall.push_back(command_pipe);
			for (int j = 0; j<32; j++){
				register_wait[j] = std::max(0, register_wait[j] - add_line - 1);
			}
			if (add_line){
				for (auto it = address.begin(); it != address.end(); ++it){
					if (it->second >= commands_stall.size()-1-add_line) it->second += add_line;
				}
			}
			condition = forward ? (command_pipe[0] == "lw") : ((command_pipe[0] == "lw") || R_type.count(command_pipe[0]) || (command_pipe[0] == "addi"));
			if (condition) {
				if (!checkRegister(command_pipe[1])){
					handleExit(INVALID_REGISTER, commands_stall.size());
					return;
				}
				if (registerMap[command_pipe[1]] != 0) {
					if (stage) {
						register_wait[registerMap[command_pipe[1]]] = forward ? 1 : 2;
						wait_lw = (command_pipe[0] == "lw") ? registerMap[command_pipe[1]] : 0;
					}
					else if (command_pipe[0] == "lw"){
						register_wait[registerMap[command_pipe[1]]] = forward ? 2 : 4;
						wait_lw = registerMap[command_pipe[1]];
					}
					else {
						register_wait[registerMap[command_pipe[1]]] = forward ? 0 : 2;
						wait_lw = 0;
					}
				}
			}
		} 

		// // Print
		// for (int i = 0; i<commands_stall.size(); i++){
		// 	for (int j = 0; j< commands_stall[i].size(); j++){
		// 		std::cout << commands_stall[i][j] << " ";
		// 	}
		// 	std::cout << "\n";
		// }
		// for (auto it = address.begin(); it != address.end(); ++it){
		// 	std::cout << it->first << "\t" << it->second << "\n";
		// }

		if (stage) {
			PipelineStage5(forward);
		}
		else {
			PipelineStage9(forward);
		}
	}

	// print the register data in hexadecimal
	void printRegistersAndMemoryDelta(){
		for (int i = 0; i < 32; ++i)
			std::cout << registers[i] << ' ';
		std::cout << '\n';
		std::cout << memoryDelta.size() << ' ';
		for (auto &p : memoryDelta)
			std::cout << p.first << ' ' << p.second << ' ';
		std::cout << '\n';
		memoryDelta.clear();
	}
};

#endif