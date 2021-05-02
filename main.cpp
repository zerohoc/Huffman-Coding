#include "huffman.hpp"

int main()
{
	std::string fileName = "input.txt";
	std::ifstream file(fileName);

	if (!file)
	{
		std::cerr << "Error while opening the file => " << fileName << std::endl;
		system("PAUSE");
		exit(1);
	}

	std::string letterArray{};
	ReadFile(file, letterArray);
	file.close();

	//Find Frequencies
	std::cout << "Part - 1" << std::endl << std::endl;
	std::map<std::string, int> letterFreqs;
	CreateFreqMap(letterArray, letterFreqs, 1);
	PrintFreqMap(letterFreqs);

	//Build HuffmanTree
	std::cout << std::endl << "-----------------------------" << std::endl << std::endl;
	std::cout << "Binary Tree Vertically" << std::endl << std::endl;
	std::priority_queue<Node*, std::vector<Node* >, greater > minHeapForLetter;
	Node* rootLetter = BuildHuffmanTree(letterFreqs, minHeapForLetter);
	PrintBinaryTreeVertically(rootLetter);

	//Generate Codes
	std::cout << std::endl << "-----------------------------" << std::endl << std::endl;
	std::cout << "Codes:" << std::endl;
	std::map<std::string, std::string> codeMapForLetter;
	GenerateCodes(rootLetter, codeMapForLetter, "");

	//Print Total bit size
	std::cout << std::endl << "-----------------------------" << std::endl << std::endl;
	int totalSizeForLetter = FindCompressionSize(letterFreqs, codeMapForLetter);
	std::cout << "Total Size:" << std::endl;
	std::cout << totalSizeForLetter << " bits" << std::endl;
	std::cout << std::endl << "-----------------------------" << std::endl << std::endl;

	DeleteBinaryTreeFromHeap(rootLetter);

	system("PAUSE");

	return 0;
}
