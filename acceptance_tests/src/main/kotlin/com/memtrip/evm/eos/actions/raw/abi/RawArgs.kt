package com.memtrip.evm.eos.actions.raw.abi

import com.memtrip.eos.abi.writer.Abi
import com.memtrip.eos.abi.writer.AccountNameCompress
import com.memtrip.eos.abi.writer.DataCompress
import com.memtrip.eos.abi.writer.StringCompress

@Abi
data class RawArgs(
    val from: String,
    val code: String,
    val sender: String
) {

    val getFrom: String
        @AccountNameCompress get() = from

    val getCode: String
        @DataCompress get() = code

    val getSender: String
        @StringCompress get() = sender
}