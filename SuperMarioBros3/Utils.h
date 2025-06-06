#pragma once

#include <Windows.h>

#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>


using namespace std;

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

//huong cua van toc
int RandomNV();

float calculateDistance(float x1, float y1, float x2, float y2);

string ConvertNumberToString(long number, int minSize);

int RandomInRange(int a, int b);