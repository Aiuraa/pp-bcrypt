// Using y_testing doesn't work here
// It can lead to "possible crash thingy"

#include <a_samp>
#include <pp-bcrypt>

main()
{
    printf("Testing bcrypt...");

    // First we wait approx 1 second before test
    // For everything to be load first.
    wait_ms(1000);
    BCrypt_TestNormalHash();
    BCrypt_TestDynamicStrHash();
}

BCrypt_TestNormalHash()
{
    printf("Test Normal Hash begin...");

    new
        oldtime, 
        hash_str[BCRYPT_HASH_LENGTH], 
        ret, newtime;

    oldtime = GetTickCount();
    await_str(hash_str) BCrypt_AsyncHash("Hello World!");
    newtime = GetTickCount();

    printf("BCrypt_AsyncHash() took %dms to perform hashing", newtime - oldtime);

    oldtime = GetTickCount();
    ret = await BCrypt_AsyncVerify("Hello World!", hash_str);
    newtime = GetTickCount();

    printf("BCrypt_AsyncVerify() took %dms to perform checking", newtime - oldtime);

    printf("Is the result same?: %s", ret ? "Yes" : "No");
    return ret;
}

BCrypt_TestDynamicStrHash()
{
    printf("Test Dynamic String Hash begin...");

    new
        oldtime, 
        String:hash, 
        ret, newtime;

    oldtime = GetTickCount();
    hash = await_str_s BCrypt_AsyncHashStr(str_new_static("Helo World!"));
    newtime = GetTickCount();

    printf("BCrypt_AsyncHashStr() took %dms to perform hashing", newtime - oldtime);

    // Ensures the hash WILL be destroyed at the end of this functions
    pawn_guard(hash);

    oldtime = GetTickCount();
    ret = await BCrypt_AsyncVerifyStr(str_new_static("Helo World!"), hash);
    newtime = GetTickCount();

    printf("BCrypt_AsyncVerifyStr() took %dms to perform checking", newtime - oldtime);

    new hash_str[BCRYPT_HASH_LENGTH];
    str_get(hash, hash_str);

    printf("Is the result same?: %s", ret ? "Yes" : "No");
    return 0;
}
