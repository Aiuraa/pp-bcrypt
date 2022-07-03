# PawnPlus BCrypt

[![sampctl](https://img.shields.io/badge/sampctl-pp--bcrypt-2f2f2f.svg?style=for-the-badge)](https://github.com/Aiuraa/pp-bcrypt)

PawnPlus BCrypt is a library to make hashing and checking more easy with a help of `Task & Await` from PawnPlus.

This implementations originally took from Mergevos, but his version doesn't support SyS Bcrypt. Since i don't want to break his work, i decided to make this repo to distinguish between his version and mine.


## Installation

Simply install to your project:

```bash
sampctl package install Aiuraa/pp-bcrypt
```

Include in your code after PawnPlus.inc and BCrypt include, then you can begin using the library:

```pawn
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
#include <pp-bcrypt>

main()
{
    // First, get the hash using BCrypt_AsyncHash with await_str method.
    new string_hash[BCRYPT_HASH_LENGTH];
    await_str(string_hash) BCrypt_AsyncHash("Hello World!");

    // After that, we verify it using BCrypt_AsyncVerify with await method.
    new ret = await BCrypt_AsyncVerify("Hello World!", string_hash);

    // And finally, print the results here (since this is just a test, you don't have to do this)
    printf("Result hash: %s", string_hash);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
```

Dynamic String usage:
```pawn
#include <pp-bcrypt>

main()
{
    // First, get the hash using BCrypt_AsyncHashStr with await_str_s method.
    new String:hash = await_str_s BCrypt_AsyncHashStr(str_new_static("Helo World!"), 12);

    // After that, we use pawn_guard (see below for explanation)
    pawn_guard(hash);

    // Then, we verify it using BCrypt_AsyncVerifyStr with await method.
    new ret = await BCrypt_AsyncVerifyStr(str_new_static("Helo World!"), hash);

    new hash_str[BCRYPT_HASH_LENGTH];
    str_get(hash, hash_str);

    // And finally, print the results here (since this is just a test, you don't have to do this)
    printf("Result hash: %s", hash_str);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
```

#### Why pawn_guard?
In `Dynamic String` usage, using `pawn_guard` is required for making sure that the hash will be destroyed at the end of `main` function. Without this the `hash` text can be destroyed when calling `BCrypt_AsyncVerifyStr`. But if you're already using `str_acquire` and decided to make the Dynamic String to be accessed globally, then no need to use `pawn_guard`.

## Testing

To test, simply run the package:

```bash
sampctl package run
```

## Credits
* Mergevos - [async-bcrypt](https://github.com/Mergevos/samp-async-bcrypt) (It give me inspiration to make this one)
* Grabber - [pp-mysql](https://github.com/AGraber/pawn-plus-mysql) (the pp logic basically from his include)
* IllidanS4 - [PawnPlus](https://github.com/IllidanS4/PawnPlus)
