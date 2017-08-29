/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include "arkParse.h"

parserStatus_e parseTxInternal(uint8_t *data, uint32_t length, txContent_t *context) {
    // type: byte / 0
    context->type      = data[0];

    // timestamp: LE int / 1..4
    // TODO: what about sign
    context->timestamp = (uint32_t)data[4] << 24 | (uint32_t)data[3] << 16 | (uint32_t)data[2] << 8 | (uint32_t)data[1];

    // senderPublicKey / 5..37
    os_memmove(context->senderPublicKey, data + 5, 33);

    // recipientId (destination address): decoded base58check / 38..58
    os_memmove(context->recipientId, data + 38, 21);

    context->vendorFieldOffset = 59;
    // vendorField: 64 bytes hex / 59..122
    //os_memmove(context->vendorField, data + 59, 64);

    // amount: LE long / 123..130
    // TODO: what about sign
    context->amount    = (uint64_t)data[130] << 56 | (uint64_t)data[129] << 48 | (uint64_t)data[128] << 40 | (uint64_t)data[127] << 32 | (uint64_t)data[126] << 24 | (uint64_t)data[125] << 16 | (uint64_t)data[124] << 8 | (uint64_t)data[123];

    // fee: LE long / 131..138
    // TODO: what about sign
    context->fee       = (uint64_t)data[138] << 56 | (uint64_t)data[137] << 48 | (uint64_t)data[136] << 40 | (uint64_t)data[135] << 32 | (uint64_t)data[134] << 24 | (uint64_t)data[133] << 16 | (uint64_t)data[132] << 8 | (uint64_t)data[131];

    // asset, meaning depending on type:
    context->assetlength = length-139;
    context->assetOffset = 139;
    
    // 0: transfer -> no asset
    if(context->type == 0){
      if(context->assetlength != 0) return USTREAM_FAULT;
    }
    // 1: second signature -> publicKey (of the secondsignature) 139..171
    else if(context->type == 1){
      if(context->assetlength != 33) return USTREAM_FAULT;      
      //os_memmove(context->asset, data + 139, 33);
    }
    // 2: delegate registration -> username serialized in utf8
    else if(context->type == 2){
      if(context->assetlength > 40) return USTREAM_FAULT;
      //os_memmove(context->asset, data + 139, context->assetlength);
    }
    // 3: vote -> publicKeys serialized in utf8 (for instance "+0345.....")
    else if(context->type == 3){
      // context->assetlength should be a multiple of (33*2 + 1) = 67
      if(context->assetlength % 67 != 0) return USTREAM_FAULT;
      context->voteSize = (context->assetlength / 67);
      // TODO: context->asset[i*67] should be utf8 of "+" or "-"
      //os_memmove(context->asset, data + 139, context->assetlength);
    }
    // 4: multisignature registration -> min, lifetime, keys in utf8 :(
    else if(context->type == 4){
      // (context->assetlength - 2) should a multiple of 33*2
      // context->assetlength >= 2 + 66*2 = 134
      if(context->assetlength < 134 || context->assetlength % 66 != 2) return USTREAM_FAULT;
      //os_memmove(context->asset, data + 139, context->assetlength);
      // TODO:
      // min = context->asset[0];
      // lifetime = context->asset[1];
      // os_memmove(keys, context->asset + 2, context->assetlength - 2);
    }
    // 5: ipfs -> no asset
    else if(context->type == 5){
      if(context->assetlength > 0) return USTREAM_FAULT;
    }
    // unknown type : no fail for future compatibility
    else {
      return USTREAM_FINISHED;
    }

    return USTREAM_FINISHED;
}

parserStatus_e parseTx(uint8_t *data, uint32_t length, txContent_t *context) {
    parserStatus_e result;
    BEGIN_TRY {
        TRY {
            os_memset(context, 0, sizeof(txContent_t));
            result = parseTxInternal(data, length, context);
        }
        CATCH_OTHER(e) {
            result = USTREAM_FAULT;
        }
        FINALLY {
        }
    }
    END_TRY;
    return result;
}
