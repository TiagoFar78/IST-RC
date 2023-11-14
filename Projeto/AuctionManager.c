/*
 * Return:
 * 1 - user was registered after this login
 * 0 - password is correct match
 * -1 - password is an incorrect match
 */
int login(int uID) {
    // Para armazenar as passwords podiamos fazer uma hashtable onde key = UID e object = password
    // Para manter o registo dos gajos logged in pode ser uma lista com os UIDs ou então uma hashtable para ser mais rápido
    return 0;
}

/*
 * Return:
 * 0 - logged out successfully
 * -1 - not logged in
 * -2 - not registered
 */
int logout(int uID) {
    // Basta remover o UID do zezoca da lista/hashtable dos gajos logged in
    return 0;
}