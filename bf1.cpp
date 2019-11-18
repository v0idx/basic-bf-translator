//Brainfuck to C Translator
//Elliott Nash, v0idx.com



#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

std::vector<std::string> getLines(std::string fileName) {
	std::vector<std::string> returnVec;
	std::fstream file(fileName);
	if (!file) {
		std::cerr << "COULDN'T READ FILE '" << fileName << "." << std::endl;
		return returnVec;
	}
	
	std::string line;

	while (std::getline(file, line)) {
		if (line.size() > 0) {
			returnVec.push_back(line);
		}
	}

	file.close();

	return returnVec;
}

std::vector<char> commandSplit(std::vector<std::string> programLines) {
	std::vector<char> commands;
	for (std::string str : programLines) {
		for (char c : str) {
			commands.push_back(c);
		}
	}
	return commands;
}

std::vector<std::string> parseCommands(std::vector<char> commandList) {
	std::vector<std::string> transCommands;
	for (char c : commandList) {
		switch(c) {
		case '<': 
			transCommands.push_back("--ptr;");
			break;
		case '>':
			transCommands.push_back("++ptr;");
			break;
		case '+':
			transCommands.push_back("++*ptr;");
			break;
		case '-':
			transCommands.push_back("--*ptr;");
			break;
		case '.':
			transCommands.push_back("putchar(*ptr);");
			break;
		case ',':
			transCommands.push_back("*ptr=getchar();");
			break;
		case '[':
			transCommands.push_back("while (*ptr) {");
			break;
		case ']':
			transCommands.push_back("}");
		}
	}
	return transCommands;
}

template <typename T> void printVec(T vec, std::string fileName) {
	std::ofstream file(fileName);

	for (typename T::value_type x : vec) {
		file << x << '\n';
	}

	file.close();
}

std::vector<std::string> importCommands(std::string fileName) {
	std::vector<std::string> programLines = getLines(fileName);
	programLines = parseCommands(commandSplit(programLines));
	return programLines;
}

std::vector<std::string> translateInit(std::string streamSize) {
	std::vector<std::string> returnVec;
	returnVec.push_back("int main() {");
	returnVec.push_back("char array[" + streamSize + "] = { 0 };");
	returnVec.push_back("char *ptr = array;");

	return returnVec;
}

int main(int argc, char* argv[]) {
	//Parse all commands from input brainfuck program
	std::vector<std::string> programLines = importCommands(argv[1]);

	//Initialise the c translation
	std::vector<std::string> transLines = translateInit(argv[2]);

	//Add all of the translated commands to the new vector
	for (std::string str : programLines) {
		transLines.push_back(str);
	}

	//Close out the int main() {}
	transLines.push_back("return 0; }");

	//Write to file
	printVec(transLines, "main.c");


}

