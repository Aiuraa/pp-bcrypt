
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

    BCrypt_BeginTesting();
    BCrypt_BeginTestingStr();
}

BCrypt_BeginTesting()
{
    new x_result[BCRYPT_HASH_LENGTH];
    await_arr(x_result) BCrypt_AsyncHash("Hello World!");
    new ret = await BCrypt_AsyncVerify("Hello World!", x_result);

    printf("Result naked: %s", x_result);
    printf("Result same?: %s", ret ? "Yes" : "No");
}

BCrypt_BeginTestingStr()
{
    new String:text = str_new_static("Helo World!");
    pawn_guard(text);

    new String:hash = await_str_s BCrypt_AsyncHashStr(text, 12);
    pawn_guard(hash);

    new hash_str[BCRYPT_HASH_LENGTH];
    str_get(hash, hash_str);
    printf("Result naked: %s", hash_str);

    new ret = await BCrypt_AsyncVerifyStr(text, hash);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
