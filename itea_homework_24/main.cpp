#include <iostream>
#include <map>
#include <cctype>

class BaconCipher {
private:
	static std::map<char, std::string> encoding_dict;
	static std::map<std::string, char> decoding_dict;
	static std::string                 masking_text ;
private:
	static void maps_filling() {
		if (encoding_dict.size() == 0) {
			char key[] = "aaaaabbbbbabbbaabbababbaaababaab";
			for (char i = 'a'; i <= 'z'; i++) {
				int index = i - 'a';
				encoding_dict[i] = std::string(&key[index], 5);
				decoding_dict[std::string(&key[index], 5)] = i;
			}
		}
	}
	
	static size_t only_letters_length(const std::string& str) {
		size_t rez = 0;

		size_t len = str.length();
		for (size_t i = 0; i < len; ++i) {
			char ch = str[i];
			ch = tolower(ch);
			if ('a' <= ch && ch <= 'z') {
				rez++;
			}
		}		
		return rez;
	}

public:
	static size_t maximum_length_can_encoded() {
		size_t rez = only_letters_length(masking_text);		
		rez = rez / 5;
		return rez;
	}

	static std::string encode(const std::string& plaintext, bool* is_ok = nullptr) {
        maps_filling();
		
		if ( maximum_length_can_encoded() < only_letters_length(plaintext) ) {
			if (is_ok) {
				*is_ok = false;
			}
			return std::string("The maximum permissible length of the coded string has been exceeded.");
		}

		std::string groupsAB_text;		

		size_t len = plaintext.length();
		for (size_t i = 0; i < len; ++i) {
			char ch = plaintext[i];
			if (encoding_dict.find(ch) != encoding_dict.end()) {
				groupsAB_text += encoding_dict[ch];
			}
		}
		
		// - - - - - - - - - - - - - - - - - - - - - - - - - - -
		std::string encoded_text;
		len = groupsAB_text.length();
		int j = -1;
		int i =  0;
		while (true)
		{	
			char ch_s = masking_text[i];
			ch_s = tolower(ch_s);
			if ('a' <= ch_s && ch_s <= 'z') {
				j++;
				char ch_f = groupsAB_text[j];
				if (ch_f == 'a') {
					encoded_text += tolower(ch_s);
				}
				else {
					encoded_text += toupper(ch_s);
				}				
			}
			else {
				encoded_text += ch_s;			
			}
			i++;
			if (len == j) {
				break;
			}
		}
		if (is_ok) {
			*is_ok = true;
		}
		return encoded_text;
	}
	
	static std::string decode(const std::string& ciphertext) {		
		maps_filling();

		std::string str;
		size_t len = ciphertext.length();
		for (size_t i = 0; i < len; ++i) {
			char ch_s = ciphertext[i];
			ch_s = tolower(ch_s);
			if ('a' <= ch_s && ch_s <= 'z') {
				if (ciphertext[i] == ch_s) {
					str += 'a';
				}
				else {
					str += 'b';
				}
			}
		}
		
		len = str.length();
		std::string decoded_text;
		for (size_t i = 0; i < len; i += 5) {
			std::string group = str.substr(i, 5);
			if (decoding_dict.find(group) != decoding_dict.end()) {
				decoded_text += decoding_dict[group];
			}
		}

		return decoded_text;
	}
	
};

std::map<char, std::string> BaconCipher::encoding_dict = {};
std::map<std::string, char> BaconCipher::decoding_dict = {};
std::string BaconCipher::masking_text = "Welcome to the Hotel California Such a lovely place Such a lovely place";

int main() {

	std::string plaintext = "homework";
	bool is_ok = false;
	std::string ciphertext = BaconCipher::encode(plaintext, &is_ok);	
	std::cout << "Encoded Text: " << ciphertext << std::endl;

	if (is_ok)
	{
		std::string decoded_text = BaconCipher::decode(ciphertext);
		std::cout << "Decoded Text: " << decoded_text << std::endl;
	}

	return 0;
}
