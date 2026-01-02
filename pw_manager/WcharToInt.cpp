//#include<windows.h>

int WcharToInt(const wchar_t* str) {
	int result = 0;

	
	while (*str) {
		if (*str < L'0' || *str > L'9') break;
		
		result = result * 10 + ( *str - L'0' ); 
		str++;
	}






	return result;
}