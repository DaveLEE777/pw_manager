#include<windows.h>
#include<wchar.h>

void ByteToChar(BYTE* fromByte, wchar_t* toChar, int size) {
	const wchar_t charset[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ" L"abcdefghijklmnopqrstuvwxyz" L"0123456789" L"!@#$%^&*";
	
	for (int i = 0; i < size; i++) {
		toChar[i] = charset[fromByte[i] % (sizeof(charset) / sizeof(wchar_t) - 1)];
	}
	toChar[size] = L'\0';
}