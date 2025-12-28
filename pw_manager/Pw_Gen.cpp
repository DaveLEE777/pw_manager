#include<windows.h>
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")

#include "Pw_Gen.h"

void Pw_Gen(BYTE* buffer, DWORD size) {
	BCryptGenRandom(
		NULL,
		buffer,
		size,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	);
}


