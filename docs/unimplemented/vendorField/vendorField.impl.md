
# VendorField Implementation

There was some weird bug tied somehow to the length of the vendorfield.

VF of `0.12711620700894244` would cause `0.1` to overlay the left-side `BAGL_GLYPH_ICON_CROSS` icon.


If the length was shorter, (e.g. `0.12711620`), there would be no overlay bug.

A longer vendorField (`0.127116207008942440.127116207008942440.127116207008942440.12711620700894244`) would not also not cause an icon overlay bug.


If the display slot-order was changed (title[0]:var[0]==VF, title[1]:var[1]==To), the icon bug still only shows on the vendorField screen.


The following are snippets from the implementation for future reference.

```c
static void setVendorField(const Transaction *transaction) {
    os_memmove((char *)displayCtx.title[1], "VendorField\0", 12U);

    uint8_t vfLength = MIN(transaction->vendorFieldLength, HASH_64_LENGTH);

    os_memmove((char *)displayCtx.var[1], (char *)transaction->vendorField, vfLength);
    displayCtx.var[1][vfLength] = '\0';

    if (transaction->vendorFieldLength > HASH_64_LENGTH) {
        os_memmove((char *)&displayCtx.var[1][HASH_64_LENGTH], (char *)"...\0", 4U);
    }
}
```

```c
void setDisplayTransfer(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "Transfer\0", 9U);
    os_memmove((char *)displayCtx.title[0], "To\0", 3U);

    // Lets offset 'Amount' & 'Fees' by '+1' if there's a VendorField.
    // Display slot offset 0 if No VF; 1 if VF
    uint8_t offset = (transaction->vendorFieldLength != 0);

    os_memmove((char *)displayCtx.title[1U + offset], "Amount\0", 7U);
    os_memmove((char *)displayCtx.title[2U + offset], "Fees\0", 5U);

    // Recipient
    // os_memset(displayCtx.var[0], 0, sizeof(displayCtx.var[0]));
    encodeBase58PublicKey((uint8_t *)transaction->asset.transfer.recipient,
                          ADDRESS_HASH_LENGTH,
                          (uint8_t *)displayCtx.var[0],
                          sizeof(displayCtx.var[0]),
                          transaction->asset.transfer.recipient[0],
                          1U);
    displayCtx.var[0][ADDRESS_LENGTH] = '\0';

    // Amount
    printAmount(transaction->asset.transfer.amount,
                displayCtx.var[1U + offset], sizeof(displayCtx.var[1U + offset]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.var[2U + offset], sizeof(displayCtx.var[2U + offset]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);

    setDisplaySteps(transaction->vendorFieldLength ? 4U : 3U);
}
```
> 'transactions/display_transaction.c'


```c
static void internalDeserializeCommon(Transaction *transaction,
                                      const uint8_t *buffer) {
    transaction->header             = buffer[0];
    transaction->version            = buffer[1];
    transaction->type               = U2LE(buffer, 7U);
    os_memmove(transaction->senderPublicKey, &buffer[17], 33U);
    transaction->fee                = U8LE(buffer, 50U);
    transaction->vendorFieldLength  = buffer[58];

    if (transaction->vendorFieldLength) {
        os_memmove(transaction->vendorField,
                   &buffer[58U + 1U],
                   MIN(transaction->vendorFieldLength, HASH_64_LENGTH));
    }
}
```
> 'transactions/deserializer.c'

```c
typedef struct transaction_t {
    uint8_t     header;
    uint8_t     version;
    uint16_t    type;
    uint8_t     senderPublicKey[PUBLICKEY_COMPRESSED_LENGTH];
    uint64_t    fee;
    union {
        struct {  // v1 or Legacy
            uint8_t     recipient[ADDRESS_HASH_LENGTH];
            uint32_t    vendorFieldOffset;
            uint64_t    amount;
            uint32_t    assetOffset;
            uint8_t     assetLength;
            uint8_t     *assetPtr;
        };
        struct {  // v2
            uint8_t     vendorFieldLength;
            uint8_t     vendorField[HASH_64_LENGTH];
            tx_asset_t  asset;
        };
    };
} Transaction;
```



## Ledger Test Commands

#### With VendorField

```shell
python examples/example_helper.py --tx ff0217010000000000020000000000000003d59f3b7d698536f6925a77f22d484d518b06a2c09318e8e5ff487afcdedefb2c809698000000000013302e3132373131363230373030383934323434010000000000000000000000178c9bd74222025a19063c8fca8a50c39a891feeca
```

#### With long VendorField

```shell
python examples/example_helper.py --tx ff0217010000000000020000000000000003d59f3b7d698536f6925a77f22d484d518b06a2c09318e8e5ff487afcdedefb2c80969800000000004c302e3132373131363230373030383934323434302e3132373131363230373030383934323434302e3132373131363230373030383934323434302e3132373131363230373030383934323434010000000000000000000000178c9bd74222025a19063c8fca8a50c39a891feeca
```

#### No VendorField

```shell
python examples/example_helper.py --tx ff0217010000000000010000000000000003a02b9d5fdd1307c2ee4652ba54d492d1fd11a7d1bb3f3a44c4a05e79f19de933809698000000000000a08601000000000000000000171dfc69b54c7fe901e91d5a9ab78388645e2427ea
```
