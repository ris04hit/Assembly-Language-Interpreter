compile: 5stage.cpp 5stage_bypass.cpp 79stage.cpp 79stage_bypass.cpp MIPS_Processor.hpp
	g++ 5stage.cpp MIPS_Processor.hpp -o 5stage.exe
	g++ 5stage_bypass.cpp MIPS_Processor.hpp -o 5stage_bypass.exe
	g++ 79stage.cpp MIPS_Processor.hpp -o 79stage.exe
	g++ 79stage_bypass.cpp MIPS_Processor.hpp -o 79stage_bypass.exe

run_5stage: 5stage.exe input.asm
	./5stage.exe input.asm

run_5stage_bypass: 5stage_bypass.exe input.asm
	./5stage_bypass.exe input.asm

run_79stage: 79stage.exe input.asm
	./79stage.exe input.asm

run_79stage_bypass: 79stage_bypass.exe input.asm
	./79stage_bypass.exe input.asm