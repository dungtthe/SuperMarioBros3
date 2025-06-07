#include <Windows.h>

#include "Utils.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring *w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}


int RandomNV() {
	srand(time(0));
	int n = (rand() % 2) * 2 - 1;
	return n;
}

float calculateDistance(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}

string ConvertNumberToString(long number, int minSize)
{
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(minSize) << number;
	return oss.str();
}

//[a,b]
int RandomInRange(int a, int b) {
	static std::random_device rd;  
	static std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dist(a, b); 
	return dist(gen);
}