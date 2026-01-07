#include<windows.h>
#include "DB.h"
#include "sqlite3.h"
#include<CommCtrl.h>

static sqlite3* g_db = nullptr;

bool DB_Init() {
	if (sqlite3_open("list.db", &g_db) != SQLITE_OK) {
		return false;
	}

	const char* sql = "CREATE TABLE IF NOT EXISTS list (id TEXT, pw TEXT, site TEXT);";
	
	return sqlite3_exec(g_db, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool DB_Save(const wchar_t* id, const wchar_t* pw, const wchar_t* site) {
	sqlite3_stmt* stmt;
	const char* sql = "INSERT INTO list(id, pw, site) VALUES(?, ?, ?);";
	if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
		return false;

	sqlite3_bind_text16(stmt, 1, id, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text16(stmt, 2, pw, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text16(stmt, 3, site, -1, SQLITE_TRANSIENT);

	bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return ok;

}

bool DB_Load_All(HWND hListView)
{
    const char* sql = "SELECT site,id,pw FROM list;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    ListView_DeleteAllItems(hListView);

    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const wchar_t* site = (const wchar_t*)sqlite3_column_text16(stmt, 0);
        const wchar_t* id = (const wchar_t*)sqlite3_column_text16(stmt, 1);
        const wchar_t* pw = (const wchar_t*)sqlite3_column_text16(stmt, 2);

        LVITEM item = {};
        item.mask = LVIF_TEXT;
        item.iItem = index;
        item.pszText = const_cast<LPWSTR>(id);
        ListView_InsertItem(hListView, &item);

        ListView_SetItemText(hListView, index, 1, const_cast<LPWSTR>(site));
        ListView_SetItemText(hListView, index, 2, const_cast<LPWSTR>(pw));

        index++;
    }

    sqlite3_finalize(stmt);
    return true;
}

void DB_Close() {
	if (g_db) sqlite3_close(g_db);
}