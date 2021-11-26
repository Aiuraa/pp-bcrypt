
#include <a_samp>

#define PP_SYNTAX_GENERIC
#define PP_SYNTAX_AWAIT

#include <PawnPlus>
#include <samp_bcrypt>

//#include <bcrypt>

#include "pp-bcrypt.inc"

main()
{
    printf("Testing bcrypt...");

    // Using y_testing doesn't work here
    // It can lead to "possible crash thingy"
    BCrypt_TestNormalHash();
    BCrypt_TestDynamicStrHash();
}

BCrypt_TestNormalHash()
{
    new x_result[BCRYPT_HASH_LENGTH];
    await_str(x_result) BCrypt_AsyncHash("Hello World!");
    new ret = await BCrypt_AsyncVerify("Hello World!", x_result);

    printf("Result naked: %s", x_result);
    printf("Result same?: %s", ret ? "Yes" : "No");
}

BCrypt_TestDynamicStrHash()
{
    new String:hash = await_str_s BCrypt_AsyncHashStr(str_new_static("Helo World!"), 12);
    pawn_guard(hash);

    new ret = await BCrypt_AsyncVerifyStr(str_new_static("Helo World!"), hash);

    new hash_str[BCRYPT_HASH_LENGTH];
    str_get(hash, hash_str);

    printf("Result naked: %s", hash_str);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
