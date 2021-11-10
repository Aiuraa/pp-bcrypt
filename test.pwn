
#include <a_samp>

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
    //pawn_call_native("bcrypt_hash", "dsSdsi", 0, "OnSomething", str_new_static("Hello World!"), BCRYPT_COST, "i", 12);
    new x_result[BCRYPT_HASH_LENGTH];
    await_arr(x_result) BCrypt_AsyncHashStr(str_new_static("Hello World!"), 12);
    new ret = await BCrypt_AsyncVerify("Hello World!", x_result);

    printf("Result naked: %s", x_result);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
