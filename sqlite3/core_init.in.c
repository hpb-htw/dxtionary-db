int core_init(const char* dummy)
{
        int nErr = 0;
        nErr += sqlite3_auto_extension((void (*)(void))sqlite3_spellfix_init);
        return nErr ? SQLITE_ERROR : SQLITE_OK;
}
