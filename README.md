# pp-bcrypt

[![sampctl](https://img.shields.io/badge/sampctl-pp--bcrypt-2f2f2f.svg?style=for-the-badge)](https://github.com/Kirima2nd/pp-bcrypt)

pp-bcrypt is basically a PawnPlus bcrypt with Task based await which
makes it to get the results in the same place without having to make a new callbacks.

This implementations originally made by Mergevos, but sadly his version is not supporting SyS Bcrypt.

Supports hashing for Dynamic String.

## Installation

Simply install to your project:

```bash
sampctl package install Kirima2nd/pp-bcrypt
```

Include in your code after PawnPlus.inc and BCrypt include, then you can begin using the library:

```pawn
#include <PawnPlus>
#include <samp_bcrypt>

#include <pp-bcrypt>
```

## Function Lists
```pawn
/*
    Hashing
    - returns string hash with BCRYPT_HASH_LENGTH size
    - returns Dynamic String
*/
stock Task:BCrypt_AsyncHash(const input[], cost = BCRYPT_COST);
stock Task:BCrypt_AsyncHashStr(ConstStringTag:input, cost = BCRYPT_COST);

/* 
    Verifying
    - Returns integer/number (success can be 0 and 1)
*/
stock Task:BCrypt_AsyncVerify(const input[], const hash[]);
stock Task:BCrypt_AsyncVerifyStr(ConstStringTag:input, ConstStringTag:hash);
```

## Usage

Normal string usage:
```pawn
#include <PawnPlus>
#include <samp_bcrypt>

#include <pp-bcrypt>

main()
{
    await_arr(x_result) BCrypt_AsyncHash("Hello World!");
    new ret = await BCrypt_AsyncVerify("Hello World!", x_result);

    printf("Result naked: %s", x_result);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
```

PawnPlus Dynamic String usage:
```pawn
main()
{
    new String:text = str_new_static("Helo World!");
    pawn_guard(text);

    new String:hash = await_s BCrypt_AsyncHashStr(text, 12);
    pawn_guard(hash);

    new hash_str[BCRYPT_HASH_LENGTH];
    str_get(hash, hash_str);
    printf("Result naked: %s", hash_str);

    new ret = await BCrypt_AsyncVerifyStr(text, hash);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
```

In that example, using `pawn_guard` is required since i'm storing the string as local and can be destroyed in both `BCrypt_AsyncHashStr` and `BCrypt_AsyncVerifyStr`. But if you're already using `str_acquire` and deceided to make the Dynamic String to be accessed globally, then no need to use `pawn_guard`.

## Testing

To test, simply run the package:

```bash
sampctl package run
```

## Credits
* Mergevos - [async-bcrypt](https://github.com/Mergevos/samp-async-bcrypt) (It give me inspiration to make this one)
* Grabber - [pp-mysql](https://github.com/AGraber/pawn-plus-mysql) (the pp logic basically from his include)
* IllidanS4 - [PawnPlus](https://github.com/IllidanS4/PawnPlus)
