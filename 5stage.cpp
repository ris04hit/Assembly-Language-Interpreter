#include "MIPS_Processor.hpp"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Required argument: file_name\n./MIPS_interpreter <file name>\n";
		return 0;
	}
	for (int file_num = 1; file_num < argc; file_num++){
		// std::cout << "\nRunning on " << argv[file_num] << "\n\n";
		std::ifstream file(argv[file_num]);
		MIPS_Architecture *mips;
		if (file.is_open())
			mips = new MIPS_Architecture(file);
		else
		{
			std::cerr << "File could not be opened. Terminating...\n";
			return 0;
		}

		mips->executeCommandsPipelined(false, true);
	}
	return 0;
}
