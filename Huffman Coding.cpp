# include <iostream>
# include <vector>
# include <fstream>
# include <iomanip>
# include <cmath>
using namespace std;



class Correspond {
	public :
		int value;
	  char character;
	  string huffmanCode;


  public : Correspond(char c, string s, int i) {
		character = c, huffmanCode = s, value = i;
  } // constructor
}; // class Correspond



class Node {
  public :
	  int value;
	  char character;
    Node *left, *right;
    
    
	public : Node(char c, int v) {
		value = v, character = c;
		left = right = NULL;
	} // Node(int, char)
	
	
	public : Node(char c) {
		value = 0, character = c;
		left = right = NULL;
	} // Node(char)
	
	
	public : Node(Node *l, Node *r) {
		character = '\0';
		value = l->value + r->value;
		left = l, right = r;
	} // Node(Node*, Node*)
}; // class Node



class File {
	public : ifstream inf;


	public : void openFile() {
	  string userInput_fileName;

		do {
		  cout << "Input file name ( e.g. input1.txt ) : ";
      getline(cin, userInput_fileName);
      inf.open(StringToCharPtr(userInput_fileName)); // try to open file
      
      if ( !inf ) cout << "\n### File does not exist! ###\n";
		} while( !inf ) ;
	} // openFile()


	private : char* StringToCharPtr(string s) {
	  char* fileName = new char[s.length() + 1]; // string transfer to char[]
    for ( int a = 0 ; a < s.length() ; a++ ) fileName[a] = s[a];
    fileName[s.length()] = '\0';
    return fileName;
  } // StringTransToCharPointer()
  
  
  public : void closeFile() {
	  inf.close();
	} // closeFile()
}; // calss File



class HuffmanCoding {
	public : File file;
	private : vector<Node*> charTable, huffmanTree, onlyUseForMergeSort;
						vector<Correspond> resultingTable;
	
	
	public : HuffmanCoding() {
		createCharTable();
	} // constructor
	
	
	private : void createCharTable() {
		for ( int a = 32 ; a <= 126 ; a++ ) {
			if ( !isdigit((char)a) && !( 97 <= a && a <= 122 )) {
				Node *temp = new Node(a);
		    charTable.push_back(temp);
		    
		    temp = NULL;
		    delete temp;
			} // if()
		} // for(a)
	} // createCharTable()
	
	
	public : void countChar() {
		for( int a = 0 ; a < charTable.size() ; a++ ) // reset charTable
		  charTable[a]->value = 0;

		string line;
	
		while ( getline(file.inf, line) ) {
			for ( int a = 0 ; a < line.size() ; a++ ) {
				if ( isdigit(line[a]) ) continue;
				else if ( 'a' <= line[a] && line[a] <= 'z' ) line[a--] -= 32; // a -> A
				else if ( ' ' <= line[a] && line[a] <= '/' ) charTable[line[a] - ' ' +  0]->value++;
				else if ( ':' <= line[a] && line[a] <= '`' ) charTable[line[a] - ':' + 16]->value++;
				else if ( '{' <= line[a] && line[a] <= '~' ) charTable[line[a] - '{' + 56]->value++;
			} // for(a)
		} // while()
	} // countChar()
	
	
	public : void startHuffmanCoding() {
		huffmanTree.clear();         // vector clear
		resultingTable.clear();
		onlyUseForMergeSort.clear();
		
		for ( int a = 0 ; a < charTable.size() ; a++ ) // huffmanTree copy the count that > 0 from charTable
			if ( charTable[a]->value != 0 ) {
			  huffmanTree.push_back(charTable[a]);
			  onlyUseForMergeSort.push_back(charTable[a]); // use to confirm (vector)onlyUseForMergeSort's size equal to (vector)huffmanTree's
			} // if()

		mergeSort_Divide(0, huffmanTree.size() - 1); // Merge Sort
		createHuffmanTreeRoot();
		collectHuffmanCodeFromTree(huffmanTree[0], ""); // this is the root of tree
	} // startHuffmanCoding()
	
	
	public : void print() {
		cout << "\n";
	
		for ( int a = 0 ; a < resultingTable.size() ; a++ )
			if ( resultingTable[a].character == ' ' ) cout << "space" << right << setw(15) << setfill(' ') << resultingTable[a].huffmanCode << "\n";
			else cout << "    " << resultingTable[a].character << right << setw(15) << setfill(' ') << resultingTable[a].huffmanCode << "\n";

		int sum = 0;
		for ( int a = 0 ; a < resultingTable.size() ; a++ )
			sum += resultingTable[a].value * resultingTable[a].huffmanCode.length();

		cout << "\ncompression rate = " << rounding((double)sum / (double)huffmanTree[0]->value / 8.0, 2) << "\n\n\n";
	} // print()
	
	
	private : void collectHuffmanCodeFromTree(Node *n, string s) {
		if ( n->left )  collectHuffmanCodeFromTree(n->left,  s + '0');
		if ( n->right ) collectHuffmanCodeFromTree(n->right, s + '1');
		
		if ( !n->left && !n->right ) {
			Correspond c(n->character, s, n->value);
			bool insert = false;

		  for ( int a = 0 ; a < resultingTable.size() ; a++ )
			  if ( resultingTable[a].huffmanCode.length() > s.length() ||
				   ( resultingTable[a].huffmanCode.length() == s.length() && resultingTable[a].huffmanCode > s) ) {
				  resultingTable.insert(resultingTable.begin() + a, c);
				  insert = true;
				  break;
			  } // if()

		  if ( !insert ) resultingTable.push_back(c);
		} // if()
	} // printHuffmanTree()
	
	
	private : void createHuffmanTreeRoot() {
		while ( huffmanTree.size() > 1 ) {
			Node *n = new Node(huffmanTree[0], huffmanTree[1]);
      huffmanTree.erase(huffmanTree.begin());
		  huffmanTree.erase(huffmanTree.begin());

		  bool insert = false;

		  for ( int a = 0 ; a < huffmanTree.size() ; a++ )
			  if ( huffmanTree[a]->value > n->value ) {
				  huffmanTree.insert(huffmanTree.begin() + a, n);
				  insert = true;
				  break;
			  } // if()

		  if ( !insert ) huffmanTree.push_back(n);

		  n = NULL;
		  delete n;
		} // while(1)
	} // mergeSort_Divide
	
	
	private : void mergeSort_Divide(int first, int last) {
		if ( first < last ) {
			int mid = (first + last) / 2;
			mergeSort_Divide(first, mid);
			mergeSort_Divide(mid + 1, last);
			mergeSort_Sort(first, mid, last);
		} // if()
	} // mergeSort_Divide


	private : void mergeSort_Sort(int first, int mid, int last) {
		int first1 = first  , last1 = mid;
		int first2 = mid + 1, last2 = last;
		int  index = first;

		for ( ; first1 <= last1 && first2 <= last2 ; index++ ) {
			if ( huffmanTree[first1]->value <= huffmanTree[first2]->value ) onlyUseForMergeSort[index] = huffmanTree[first1++];
			else                                                            onlyUseForMergeSort[index] = huffmanTree[first2++];
		} // for()

		for ( ; first1 <= last1 ; first1++, index++ )
		  onlyUseForMergeSort[index] = huffmanTree[first1];
		for ( ; first2 <= last2 ; first2++, index++ )
		  onlyUseForMergeSort[index] = huffmanTree[first2];

		for ( index = first ; index <= last ; index++ )
		  huffmanTree[index] = onlyUseForMergeSort[index];
	} // mergeSort_Sort
	
	
	private : double rounding(double num, int index) {
    int multiplier = pow(10, index);
    num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);

    return num;
  } // rounding()
}; // calss HuffmanCoding



int main() {
	HuffmanCoding hc;
	
	while (1) {
		hc.file.openFile();
	  hc.countChar();
	  hc.startHuffmanCoding();
	  hc.print();
	  hc.file.closeFile();
	} // while(1)
} // main()
