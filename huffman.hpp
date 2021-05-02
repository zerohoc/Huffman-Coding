#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>

/*
* Reads the input file and fills the letterArray string
*/
void ReadFile(std::ifstream & file, std::string & letterArray)
{
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	letterArray = str;
}

/*
* Generic function to create a Frequency Map
* 
* Key size => asciiCharacterSize
* function traverses the letterArray and checks every
* key if they are in the map 
* if its in the map, it increaes the value by 1
* else creates key with value 1 
* value => Frequency
*/
void CreateFreqMap(const std::string & letterArray, std::map<std::string, int> & freqMap, int asciiCharacterSize)
{
	for (int i = 0; i < letterArray.size(); i = i + asciiCharacterSize)
	{
		std::string key = letterArray.substr(i, asciiCharacterSize);

		auto mapElement = freqMap.find(key);
		if (mapElement == freqMap.end())
		{
			// not found in the map
			freqMap.insert({ key, 1 });
		}
		else 
		{
			// found in the map
			mapElement->second++;
		}
	}
}

/*
* Prints Frequencies of map keys
*/
void PrintFreqMap(std::map<std::string, int>& freqMap)
{
	std::cout << "Frequencies:" << std::endl;

	for (auto mapElement : freqMap)
	{
		std::cout << mapElement.first << " => " << mapElement.second << std::endl;
	}
}

/*
* This typedef for key,value pair
*/
typedef std::pair<std::string, int> treeNodeData;

/*
* Tree node implementation
*/
class Node
{
public:
	Node * leftChild;
	Node * rightChild;
	treeNodeData data;
	Node(treeNodeData data) :data(data), leftChild(0), rightChild(0) {}
};

// Structure of the condition
// for sorting the pair by its
// second elements
// and also to make it min heap
struct greater {
	constexpr bool operator()(
		Node * a,
		Node * b)
		const noexcept
	{
		return a->data.second > b->data.second;
	}
};

/*
* BuildHuffmanTree:
* (a) Make a leaf node for all unique character.
* (b) Generate min heap of all leaf nodes.
* (c) Get two nodes having minimum frequencies from min heap.
* (d) Make a new internal node and assign its frequency value as the summation of the frequencies of the two nodes chosen.
* (e) Make the first extracted node the left child and the second extracted node right child.
* (f) Repeat steps b, c, dand e, until there exists a single node in the developed min heap.
*/
Node * BuildHuffmanTree(std::map<std::string, int>& freqMap, std::priority_queue<Node*, std::vector<Node*>, greater > & minHeap)
{
	//part a & b
	for (auto freqMapElement : freqMap)
	{		
		Node * node = new Node(std::make_pair(freqMapElement.first, freqMapElement.second)); //part a => Make a leaf node for all unique character.
		minHeap.push(node); //part b => Generate min heap of all leaf nodes.
	}

	//part c & d & e
	while (false == minHeap.empty())
	{
		//part c => Get two nodes having minimum frequencies from min heap.
		Node * node1 = minHeap.top();
		minHeap.pop();
		Node * node2 = minHeap.top();
		minHeap.pop();

		//part d => Make a new internal node and assign its frequency value as the summation of the frequencies of the two nodes chosen.
		Node * node = new Node(std::make_pair("\"\"", (node1->data.second + node2->data.second)));

		//part e => Make the first extracted node the left child and the second extracted node right child.		
		node->leftChild  = node1;
		node->rightChild = node2;

		//part f =>  Repeat steps until there exists a single node in the developed min heap.
		if (true == minHeap.empty())
		{
			return node;
			break;
		}			

		minHeap.push(node);
	}

	return 0;
}

/*
* frees memory taken up by binary tree from heap
*/
void DeleteBinaryTreeFromHeap(Node* root)
{
	if (root == 0)
		return;

	DeleteBinaryTreeFromHeap(root->rightChild);

	DeleteBinaryTreeFromHeap(root->leftChild);

	delete root;
}

/*
* Prints a binary tree vertically starting from the root
*/
void PrintBinaryTreeVertically(Node * root, int whiteSpace = 0)
{
	if (root == 0)
		return;

	whiteSpace += 10;

	PrintBinaryTreeVertically(root->rightChild, whiteSpace);

	for (int i = 10; i < whiteSpace; i++)
		std::cout << " ";
	std::cout << root->data.first << " / " << root->data.second << std::endl;

	PrintBinaryTreeVertically(root->leftChild, whiteSpace);
}

/*
* traverses binary tree recursively
* creates code for every node in the tree
* except for the intermediate levels
* leftchild adds "0"
* rightchild adds "1"
*/
void GenerateCodes(Node * root, std::map<std::string, std::string> & codeMap, std::string currentCode)
{
	if (root == 0)
		return;

	GenerateCodes(root->leftChild, codeMap, currentCode + "0");

	if ("\"\"" != root->data.first)
	{
		codeMap.insert(std::make_pair(root->data.first, currentCode));
		//std::cout << root->data.first << " => " << currentCode << std::endl;
	}
	

	GenerateCodes(root->rightChild, codeMap, currentCode + "1");
}

/*
* Compression = SumForAllCodes(freq * codeBitSize)
*/
int FindCompressionSize(std::map<std::string, int>& freqMap, std::map<std::string, std::string> & codeMap)
{
	int totalSize = 0;
	for (auto freqMapElement : freqMap)
	{
		auto codeMapElement = codeMap.find(freqMapElement.first);
		int freq = freqMapElement.second;
		int codeBitSize = codeMapElement->second.size();
		totalSize += freq * codeBitSize;
	}

	return totalSize;
}
