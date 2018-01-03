#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <math.h>
#include <iomanip>

void menuInput();
void generateKeyContainer();
bool generateKey();
void encryptMsg();
void decryptMsg();
bool inputVerify(char selection);
unsigned long long primePicker();
unsigned long long gcd(unsigned long long a, unsigned long long b);
unsigned long long modInverse(unsigned long long e, unsigned long long k);
unsigned long long gcdE(unsigned long long e, unsigned long long k, unsigned long long *x, unsigned long long *y);
unsigned long long crypto(unsigned long long n, unsigned long long e, unsigned long long m);
void testZone();

int main () {
	
	//Clear console & seed rand
	printf("\033c");
	srand (time(NULL));
	
	menuInput();
	
	return 0;
}

void menuInput(){
	
	std::cout << "Wilson Blaikie's RSA Implementation Ver 0.1" << std::endl;
	std::cout << "Please select" << std::endl;
	std::cout << "g: Generate Keys, e: Encryption, d: Decryption, p: Translate Plaintext, n: Translate Number, q: Quit" << std::endl;
	
	char selection;
	std::cin >> selection;
	
	//Check for specific input & run related function
	if (selection == 'g') {	
		generateKeyContainer();
	} else if (selection == 'e') {
		encryptMsg();
	} else if (selection == 'd') {
		decryptMsg();
	} else if (selection == 'q') {
		std::cout << "Quitting..." << std::endl;
		return;
	//PRODUCTION IMPLEMENTATION FUNCTION
	} else if (selection == 't') {
		testZone();
	//REMOVE AFTER COMPLETION
	} else {
		std::cout << "ERROR: Invalid Input" << std::endl;
		menuInput();
	}
}

void generateKeyContainer(){
	//Handle potential errors
	while (generateKey() == false) {
		generateKey();
	}
	menuInput();
}

bool generateKey() {
	//IDEA: Add an advanced key generation that display all steps and provides all numbers for reference & then do a simple one that just spits out n,e,d
	std::cout << std::endl;
	printf("\033c");
	std::cout << "Wilson Blaikie's RSA Implementation Ver 0.1" << std::endl;
	std::cout << std::endl;
	std::cout << "Key Generation" << std::endl;
	std::cout << "Calculating primes" << std::endl;
	//Pick 2 large primes
	unsigned long long p = primePicker();
	unsigned long long q = primePicker();
	//Find prime product
	unsigned long long n = p*q;
	//Find k for e testing
	unsigned long long k = (p - 1) * (q - 1);
	//Use generic e
	unsigned long long e = 65537;
	
	
	// EXAMPLE INPUT: [BASE 27 (a-z+space)]
	// abcdefghijklmnopqrstuvwxyz_
	// 123456789012345678901234560
	// TE = 2005
	// ST = 1920
	
	unsigned long long m1 = 2005;
	unsigned long long m2 = 1920;
	
	//Run tests on keys
	if(m1 > n || m2 > n) {
		std::cout << "ERROR: A test message value is higher than modulus, increase order" << std::endl;
		return false;
	}
	
	//Check if e & k are coprime
	if (gcd(k, e) == 1) {
		std::cout << "Co-primality achieved" << std::endl;
	} else {
		std::cout << "ERROR: Co-primality not achieved" << std::endl;
		return false;
	}	
	
	//Find d through modular inverse of e to mod k
	unsigned long long d = modInverse(e, k);
	
	//Incase inverse not found, yet to be an issue however
	if (d == 1){
		std::cout << "Inverse unattainable, re-executing" << std::endl;
		generateKey();
	}
	
	std::cout << "SUCCESS: Modular multiplicative inverse identified, private key generated" << std::endl;
	
	std::cout << "Encrypting test messages" << std::endl;
	//Use modular exponentiation to encrypt message
	unsigned long long c1 = crypto(n, e, m1);
	unsigned long long c2 = crypto(n, e, m2);
	
	std::cout << "Test messages encrypted, now testing decryption" << std::endl;
	//Use modular exponentiation to decrypt encryption
	c1 = crypto(n, d, c1);
	c2 = crypto(n, d, c2);
	
	//Check is encrypted & then decrypted message matches original
	if (c1 == m1 && c2 == m2) {
		std::cout << "Decrypted successfully, valid keys generated" << std::endl;
	} else {
		//Given that bad keys are used just restart process
		std::cout << "ERROR: Decryption unsuccessful, keys not valid" << std::endl;
		std::cout << "Reattempting key generation" << std::endl;
		return false;
	}
	
	std::cout << std::endl;
	
	std::cout << "[PUBLIC  KEY] n: " << n << ", e: " << e << std::endl;
	std::cout << "[PRIVATE KEY] d: " << d << std::endl;
	
	std::cout << std::endl;
	return true;
}

void encryptMsg() {
	bool prompt = true;
	unsigned long long inputMod = 0;
	std::vector<unsigned long long> intList; 
	
	//Prompt for key
	std::cout << "Please enter public key component n: ";
	std::cin >> inputMod;
	
	while (prompt != false) {
		unsigned long long convert = 0;
	
		//Prompt for number
		std::cout << std::endl;
		std::cout << "Enter a number for encryption m: ";
		std::cin >> convert;
	
		convert = crypto(inputMod, 65537, convert);
	
		std::cout << "Message encrypted" << std::endl;
		intList.push_back(convert);
		
		//Prompt for additional input
		char selection;
		bool ver = false;
		std::cout << "Are there any additional inputs (y/n)" << std::endl;
		
		while (ver != true) {
			std::cin >> selection;
			ver = inputVerify(selection);
		}
		
		if (selection == 'n') {
			prompt = false;
		}
	}
	
	//PRINT ALL OUTPUTS
	std::cout << std::endl;
	std::cout << "Encryption List" << std::endl;
	for (int i = 0; i < intList.size(); i++){
		std::cout << intList[i] << std::endl;
	}
	std::cout << std::endl;
	menuInput();
	
}

void decryptMsg() {
	bool prompt = true;
	unsigned long long inputMod = 0;
	unsigned long long inputD = 0;
	std::vector<unsigned long long> intList;
	
	//Prompt for key
	std::cout << "Please enter public key component n: ";
	std::cin >> inputMod;
	
	//Prompt for d
	std::cout << "Please enter private key component d: ";
	std::cin >> inputD;
	
	while (prompt != false) {
		unsigned long long convert = 0;
	
		//Prompt for number
		std::cout << std::endl;
		std::cout << "Enter a number for decryption c: ";
		std::cin >> convert;
	
		convert = crypto(inputMod, inputD, convert);
	
		std::cout << "Message decrypted" << std::endl;
		intList.push_back(convert);
			
		//Prompt for additional input
		char selection;
		bool ver = false;
		std::cout << "Are there any additional inputs (y/n)" << std::endl;
		
		while (ver != true) {
			std::cin >> selection;
			ver = inputVerify(selection);
		}
		
		if (selection == 'n') {
			prompt = false;
		}
	}
	
	//PRINT ALL OUTPUTS
	std::cout << std::endl;
	std::cout << "Encryption List" << std::endl;
	for (int i = 0; i < intList.size(); i++){
		std::cout << intList[i] << std::endl;
	}
	std::cout << std::endl;
	menuInput();
	
}

bool inputVerify(char selection) {
	//Check for unexpected input
	if (selection == 'n' || selection == 'y') {	
		return true;
	} else {
		std::cout << "Invalid input, please enter an accepted character (y/n)" << std::endl;
		return false;	
	}
	
}

//Random prime generator
unsigned long long primePicker(){
	unsigned long long possible = rand() % 100000 + 10000;
	
	if (crypto(possible, (possible - 1), 2) == 1) {
		return possible;
	} else {
		primePicker();
	}
}
	
//Recursive GCD
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    if (b != 0) {
       return gcd(b, a % b);
	} else {
       return a;
	}
}

//CURRENT MODINVERSE IS NAIVE [O(n)] USE EXTENDED EUCLIDEAN [O(log n)] IN UPDATE
unsigned long long modInverse(unsigned long long e, unsigned long long k) {
	unsigned long long x, y;
	unsigned long long g = gcdE(e, k, &x, &y);
	if (g != 1){
		return 1;
	}
	// m is added to handle negative x
	unsigned long long res = (x%k + k) % k;
	return res;
}

unsigned long long gcdE(unsigned long long e, unsigned long long k, unsigned long long *x, unsigned long long *y) {
	// Base Case
	if (e == 0)	{
		*x = 0;
		*y = 1;
		return k;
	}

	unsigned long long x1, y1; // To store results of recursive call
	unsigned long long gcd = gcdE(k % e, e, &x1, &y1);

	// Update x and y using results of recursive
	// call
	*x = y1 - (k / e) * x1;
	*y = x1;

	return gcd;
}

//Modular Exponentiation in O(log e)
unsigned long long crypto(unsigned long long n, unsigned long long e, unsigned long long m) {
	unsigned long long res = 1;
	while (e > 0) {
		if (e % 2 == 1) {
			res = (res * m) % n;
		}
		e = e >> 1;
		m = (m * m) % n;
	}
	return res;
}

void testZone() {
	/*
	//Initialise converters
	int exp = 0;
	unsigned long long seg = 0;
	int intermed[4] = {0, 0, 0, 0};
	std::vector<std::string> convec;
	std::vector<unsigned long long> intvec;
	
	//Convert string into n number of x*10^3 sub-arrays
	while (convert > 0) {
		signed int endstr = 0;
		//If remaining string is smaller than default carriage size
		if (convert.size() < 4) {
			convert.copy(intermed, convert.size(), 0);
			convec.insert(convec.begin(), intermed);
			
			//Remove all remaining chars
			while (convert.size() > 0) {
				convert.pop_back();
			}
			
		//If string is default carriage size	
		} else {
			endstr = convert.size() - 4;
			convert.copy(intermed, 4, endstr);
			convec.insert(convec.begin(), intermed);
			
			for (int i = 0; i < 4; i++) {
				convert.pop_back();
			}
		}
		for (int i = 0; i < 4; i++) {
			intermed[i] = 0;
		}
	}
	
	//Convert into int type for modinv
	unsigned long long sequence = 0;
	
	for (int i = 0; i < convec.size(); i++) {
		sequence = 0;
		convert = convec[i];
		for (int i = 0; i < convert.size(); i++) {
			sequence = (sequence * 100) + (convert[i] - 31);
		}
		intvec.push_back(sequence);
		convec[i] = "";
	}
	
	//Modinv vector elements
	for (int i = 0; i < intvec.size(); i++) {
		intvec[i] = crypto(inputMod, 65537, intvec[i]);
	}
	
	//Turn intvec elements back into plaintext
	int int2char = 0;
	char addChar = 0;
	for (int i = 0; i < intvec.size(); i++) {
		while (intvec[i] > 0) {
			int2char = 0;
			//Translate individual chars
			for (int j = 0; j < 2; j++) {
				if (j == 0) {
					int2char = int2char + (intvec[i] % 10);
				} else {
					int2char = int2char + ((intvec[i] % 10) * 10);
				}
				intvec[i] = intvec[i] / 10;
			}
			addChar = int2char + 31;
			convec[i].insert(0, 1, addChar);
		}
	}
	
	for (int i = 0; i < convec.size(); i++) {
		std::cout << convec[i] << std::endl;
	}
}
	*/
}