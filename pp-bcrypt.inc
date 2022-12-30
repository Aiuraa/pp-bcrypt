
/*
    PawnPlus bcrypt (v2.2.2)
    By: Kirima

    This include support both of bcrypt plugins (Either SyS or Lassir)
    Using some tricks that actually works well with two plugins.
    
    Credits:
        - IllidanS4 (PawnPlus Creator)
        - Mergevos (Inspired from his bcrypt include)
        - Grabber (pp-mysql method that i used in here)
*/
#if !defined _PawnPlus_included
    #tryinclude <PawnPlus>
#endif
#if !defined _PawnPlus_included
    #error pp-bcrypt.inc needs PawnPlus to be installed, please download them
#endif

#if defined _INC_pp_bcrypt
    #endinput
#endif

#define _INC_pp_bcrypt

// We need to include bcrypt automatically by ourselves if the user doesn't include it.
// Finding one of two includes, and if not found then you can just give error.
#if !defined _samp_bcrypt_included
    #tryinclude <samp_bcrypt>
#endif
#if !defined _samp_bcrypt_included
    #if !defined bcrypt_included
        #tryinclude <bcrypt>
    #endif
    #if !defined bcrypt_included
        #error You need to download one of two bcrypt include before using this
    #endif
#endif

// Distinguish between string and array.
#define string: 

#if !defined BCRYPT_COST
    #define BCRYPT_COST (12)
#endif

#if !defined BCRYPT_HASH_LENGTH
    #define BCRYPT_HASH_LENGTH (60 + 1)
#endif

// Only give if lassir include available since i'm pretty sure 100% using SyS version would fail.
#if defined bcrypt_included
    native bcrypt_hash_s(ConstAmxString:input, cost = BCRYPT_COST, const callback[], const args[], {AmxString, Float, _}:...) = bcrypt_hash;
    native bcrypt_check_s(ConstAmxString:input, ConstAmxString:hash, const callback[], const args[], {AmxString, Float, _}:...) = bcrypt_check;
    native bcrypt_needs_rehash_s(ConstAmxString:input, cost = BCRYPT_COST) = bcrypt_needs_rehash;
    native bcrypt_get_hash_s(AmxString:hash) = bcrypt_get_hash;
#endif

/*
 * <summary> Convert the input to BCrypt hash asynchronously. </summary>
 * <param name="input"> The text you want to hash </param>
 * <param name="cost"> Used for how many times to round the hash (Higher value = slower but better) </param>
 * <returns> A task that can be used to get the results (using "await_str")</returns>
 *
*/
stock Task:BCrypt_AsyncHash(const string:input[], cost = BCRYPT_COST)
{
    new
        Task:hash_task = task_new(),
        Handle:hash_handle = handle_new(hash_task, .weak = true);

    #if defined _samp_bcrypt_included
    new ret = bcrypt_hash(0, "_pp@on_hashed@pp_bcrypt", input, cost, "d", _:hash_handle);
    #elseif defined bcrypt_included
    new ret = bcrypt_hash(input, cost, "_pp@on_hashed@pp_bcrypt", "dd", 0, _:hash_handle);
    #endif

    if (ret)
    {
        handle_acquire(hash_handle);
    }
    else
    {
        task_set_error_ms(hash_task, amx_err_exit, 0);
    }
    return hash_task;
}

/*
 * <summary> Convert the input to BCrypt hash asynchronously. </summary>
 * <param name="input"> The dynamic text you want to hash </param>
 * <param name="cost"> Used for how many times to round the hash (Higher value = slower but better) </param>
 * <returns> A task that can be used to get the results (using "await_str_s")</returns>
 *
*/
stock Task:BCrypt_AsyncHashStr(ConstStringTag:input, cost = BCRYPT_COST)
{
    new
        Task:hash_task = task_new(),
        Handle:hash_handle = handle_new(hash_task, .weak = true);
    
    #if defined _samp_bcrypt_included
    new ret = pawn_call_native("bcrypt_hash", "dsSds*", 0, "_pp@on_hashed_s@pp_bcrypt", String:input, cost, "d", _:hash_handle);
    #elseif defined bcrypt_included
    new ret = bcrypt_hash_s(str_addr(String:input), cost, "_pp@on_hashed_s@pp_bcrypt", "dd", 0, _:hash_handle);
    #endif

    if (ret)
    {
        handle_acquire(hash_handle);
    }
    else
    {
        task_set_error_ms(hash_task, amx_err_exit, 0);
    }
    return hash_task;
}

/*
 * <summary> Verifying between input and the hash asynchronously. </summary>
 * <param name="input"> The text you want to verify </param>
 * <param name="hash"> The hash text used for verify </param>
 * <returns> A task that can be used to get the results (using "await")</returns>
 *
*/
stock Task:BCrypt_AsyncVerify(const string:input[], const string:hash[])
{
    new
        Task:hash_check_task = task_new(),
        Handle:hash_check_handle = handle_new(hash_check_task, .weak = true);

    // Lassir include needs to pass 4 parameter (i think only two woks?)
    // Because it may contradicts with SyS reserved callback.
    #if defined _samp_bcrypt_included
    pawn_register_callback("_pp@on_verifyhashfix@pp_bcrypt", "_pp@on_hash_verify@pp_bcrypt", _, "ed", hash_check_handle);
    new ret = bcrypt_verify(0, "_pp@on_verifyhashfix@pp_bcrypt", input, hash);
    #elseif defined bcrypt_included
    new ret = bcrypt_check(input, hash, "_pp@on_hash_verify@pp_bcrypt", "dddd", 0, _:hash_check_handle, 0, 0);
    #endif

    if (ret)
    {
        handle_acquire(hash_check_handle);
    }
    else
    {
        task_set_error_ms(hash_check_task, amx_err_exit, 0);
    }
    return hash_check_task;
}

/*
 * <summary> Verifying between input and the hash asynchronously. </summary>
 * <param name="input"> The dynamic text you want to verify </param>
 * <param name="hash"> The dynamic hash text used for verify </param>
 * <returns> A task that can be used to get the results (using "await")</returns>
 *
*/
stock Task:BCrypt_AsyncVerifyStr(ConstStringTag:input, ConstStringTag:hash)
{
    new
        Task:hash_check_task = task_new(),
        Handle:hash_check_handle = handle_new(hash_check_task, .weak = true);

    // Lassir include needs to pass 4 parameter (i think only two woks?)
    // Because it may contradicts with SyS reserved callback.
    #if defined _samp_bcrypt_included
    pawn_register_callback("_pp@on_verifyhashfix@pp_bcrypt", "_pp@on_hash_verify@pp_bcrypt", _, "ed", hash_check_handle);
    new ret = pawn_call_native("bcrypt_verify", "dsSS", 0, "_pp@on_verifyhashfix@pp_bcrypt", String:input, String:hash);
    #elseif defined bcrypt_included
    new ret = bcrypt_check_s(str_addr(String:input), str_addr(String:hash), "_pp@on_hash_verify@pp_bcrypt", "dddd", 0, _:hash_check_handle, 0, 0);
    #endif

    if (ret)
    {
        handle_acquire(hash_check_handle);
    }
    else
    {
        task_set_error_ms(hash_check_task, amx_err_exit, 0);
    }
    return hash_check_task;
}

/*
 * Internal Functions
 * ------------------
 * These public function below is a internal function that retrieve data when you're hashing or verifying the hash.
 * And when this function has been called, they will give the results to `await_*`.
 *
*/
forward _pp@on_hashed@pp_bcrypt(playerid, Handle:hash_handle);
public _pp@on_hashed@pp_bcrypt(playerid, Handle:hash_handle)
{
    new
        Task:hash_task = Task:handle_get(hash_handle);

    if (handle_linked(hash_handle))
    {
        new 
            string:hash[BCRYPT_HASH_LENGTH];
        
        bcrypt_get_hash(hash);
        task_set_result_str(hash_task, hash);
    }

    handle_release(hash_handle);
    return 1;
}

forward _pp@on_hashed_s@pp_bcrypt(playerid, Handle:hash_handle);
public _pp@on_hashed_s@pp_bcrypt(playerid, Handle:hash_handle)
{
    new
        Task:hash_task = Task:handle_get(hash_handle);

    if (handle_linked(hash_handle))
    {
        new 
            string:hash[BCRYPT_HASH_LENGTH];
        
        bcrypt_get_hash(hash);
        task_set_result_str_s(hash_task, str_new(hash));
    }

    handle_release(hash_handle);
    return 1;
}

forward _pp@on_hash_verify@pp_bcrypt(CallbackHandler:id, Handle:hash_check_handle, playerid, bool:success);
public _pp@on_hash_verify@pp_bcrypt(CallbackHandler:id, Handle:hash_check_handle, playerid, bool:success)
{
    new 
        Task:hash_check_task = Task:handle_get(hash_check_handle);
    
    #if defined bcrypt_included
    success = bcrypt_is_equal();
    #endif

    if (handle_linked(hash_check_handle))
    {
        task_set_result(hash_check_task, success);
    }

    #if defined _samp_bcrypt_included
    pawn_unregister_callback(id);
    #endif

    handle_release(hash_check_handle);
    return 1;
}

/*
 * <summary> 
 *   This particular code only reserved for SyS version.
 *   It creates special callback when the server started
 *   in order to passing some additional data to another
 *   callback named `_pp@on_hash_verify@pp_bcrypt`.
 * </summary>
 * <param name=""> </param>
 * <returns> The returns always 1, since this only creates the callback </returns>
 *
*/
#if defined _samp_bcrypt_included
forward _pp@on_init@pp_bcrypt();
public _pp@on_init@pp_bcrypt()
{
    pawn_create_callback("_pp@on_verifyhashfix@pp_bcrypt", expr_true());
    return 1;
}
#endif
