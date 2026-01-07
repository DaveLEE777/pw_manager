#pragma once

bool DB_Init();
bool DB_Save(const wchar_t* id, const wchar_t* pw, const wchar_t* site);
void DB_Close();

