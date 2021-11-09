
#include <a_samp>

#define PP_SYNTAX_AWAIT

#include <PawnPlus>
#include <samp_bcrypt>

//#include <bcrypt>

#include "pp-bcrypt.inc"

new 
    x_result[BCRYPT_HASH_LENGTH];

main()
{
    printf("Testing bcrypt...");

    BCrypt_BeginTesting();
}

BCrypt_BeginTesting()
{
    await_arr(x_result) BCrypt_HashAsync("Hello World!");
    new ret = await BCrypt_CheckAsync("Hello World!", x_result);

    printf("Result naked: %s", x_result);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
