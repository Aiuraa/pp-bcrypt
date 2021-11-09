# pp-bcrypt

[![sampctl](https://img.shields.io/badge/sampctl-pp--bcrypt-2f2f2f.svg?style=for-the-badge)](https://github.com/Kirima2nd/pp-bcrypt)

pp-bcrypt is basically a PawnPlus bcrypt with Task based await which
makes it to get the results in the same place without having to make a new callbacks.

This implementations originally made by Mergevos, but sadly his version is not supporting SyS Bcrypt.

The only difference between mine and Mergevos is this version doesn't support Dynamic String because it's a lot harder
to maintain and it seems like SyS version doesn't like it at all.

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
stock Task:BCrypt_HashAsync(const input[]);
stock Task:BCrypt_CheckAsync(const input[], const hash[]);
```

## Usage

```pawn
#include <PawnPlus>
#include <samp_bcrypt>

#include <pp-bcrypt>

main()
{
    await_arr(x_result) BCrypt_HashAsync("Hello World!");
    new ret = await BCrypt_CheckAsync("Hello World!", x_result);

    printf("Result naked: %s", x_result);
    printf("Result same?: %s", ret ? "Yes" : "No");
}
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```

## Credits
* Mergevos - [async-bcrypt](https://github.com/Mergevos/samp-async-bcrypt) (It give me inspiration to make this one)
* Grabber - [pp-mysql](https://github.com/AGraber/pawn-plus-mysql) (the pp logic basically from his include)
* IllidanS4 - [PawnPlus](https://github.com/IllidanS4/PawnPlus)
