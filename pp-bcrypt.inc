
/*
    PawnPlus bcrypt (v2.0.0)
    By: Kirima2nd

    This include support both of bcrypt plugins (Either SyS or Lassir)
    Using some shitty macro thingy, but hey at least it works!.
    
    Credits:
        - IllidanS4 (PawnPlus Creator)
        - Mergevos (Inspired from his bcrypt include)
        - Grabber (pp-mysql method that i used in here)

*/

#if !defined _PawnPlus_included
    #error "Please include PawnPlus.inc first before using this."
#endif

#if defined _INC_pp_bcrypt
    #endinput
#endif

#define _INC_pp_bcrypt

#if !defined BCRYPT_COST
    #define BCRYPT_COST (12)
#endif

#if !defined BCRYPT_HASH_LENGTH
    #define BCRYPT_HASH_LENGTH (60 + 1)
#endif

#if defined PP_SYNTAX_AWAIT
    stock String:task_await_s(Task:task) return task_wait(task), task_get_result_str_s(task);
    #define await_s%0\32;%1; task_await_s%0(%1);
#endif

#if defined _samp_bcrypt_included
    native bcrypt_hash_s(playerid, const callback[], ConstAmxString:input, cost, const args[] = "", {AmxString, Float, _}:...) = bcrypt_hash;
    native bcrypt_verify_s(playerid, const callback[], ConstAmxString:input, ConstAmxString:hash) = bcrypt_verify;
#elseif defined bcrypt_included
    native bcrypt_hash_s(ConstAmxString:input, cost = BCRYPT_COST, const callback[], const args[], {AmxString, Float, _}:...) = bcrypt_hash;
    native bcrypt_check_s(ConstAmxString:input, ConstAmxString:hash, const callback[], const args[], {AmxString, Float, _}:...) = bcrypt_check;
    native bcrypt_needs_rehash_s(ConstAmxString:input, cost = BCRYPT_COST) = bcrypt_needs_rehash;
#else
    #error "Please include either one of bcrypt include first before using this."
#endif

stock Task:BCrypt_AsyncHash(const input[], cost = BCRYPT_COST)
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

stock Task:BCrypt_AsyncVerify(const input[], const hash[])
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

forward _pp@on_hashed@pp_bcrypt(playerid, Handle:hash_handle);
public _pp@on_hashed@pp_bcrypt(playerid, Handle:hash_handle)
{
    new
        Task:hash_task = Task:handle_get(hash_handle);

    if (handle_linked(hash_handle))
    {
        new 
            hash[BCRYPT_HASH_LENGTH];
        
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
            hash[BCRYPT_HASH_LENGTH];
        
        bcrypt_get_hash(hash);
        task_set_result_str_s(hash_task, str_new_static(hash));
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
    Reserved for SyS bcrypt.
*/
#if defined _samp_bcrypt_included
forward _pp@on_init@pp_bcrypt();
public _pp@on_init@pp_bcrypt()
{
    pawn_create_callback("_pp@on_verifyhashfix@pp_bcrypt", expr_true());
    return 1;
}
#endif
