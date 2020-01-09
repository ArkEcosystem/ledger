
# Type 4: MultiSignature Registration

## Signed Json Payload

```json
{
 "version": 2,
 "network": 23,
 "type": 4,
 "nonce": "2",
 "senderPublicKey": "03b593aa66b53525c5399b4af5a4f583dede1c2a46176c6796a7284ee9c0a1167f",
 "fee": "2000000000",
 "amount": "0",
 "asset": {
    "multiSignature": {
        "publicKeys": [
            "037eaa8cb236c40a08fcb9d6220743ee6ae1b5c40e8a77a38f286516c3ff663901",
            "0301fd417566397113ba8c55de2f093a572744ed1829b37b56a129058000ef7bce",
            "0209d3c0f68994253cee24b23df3266ba1f0ca2f0666cd69a46544d63001cdf150"
        ],
        "min": 2
    }
 },
 "signature": "16b09ed47ce05cc9096b87b73008f73aa51d4ddc8713d4ca0fcbcabeacfd738e0181201d55a92d5b2978d0c458684489cf133d18c521fbec431dd759d16bedbb",
 "signatures": [
    "004495d593cfb8be3293e2473acf504870d2dcf71dbee7620270e136ed63c5eef259099d225f7866178968f0c3581509d92d902914674c8f86b99eb55aaa97586e",
    "0171d86f3f6552b237dd81272a7b0da7718c4d26682255223dcf1928174082ce72b07218162938c674afe741119650135338eb3da159e0626ddab6b7851882e08b",
    "02d44d9bde77c9ea02d3516ab3263a77f4f9fbb90c30b47eba7a8bb87325edeb78dd69f914f28426e6ff661c4bc001f253130f4e7eb092a9131c8ca69dbfaff32f"
 ],
 "id": "46343c36bf7497b68e14d4c0fd713e41a737841b6a858fa41ef0eab6c4647938",
 "MultiSignatureAddress": "ATp6gFCEu4P1TWNngyD8mvg8UYdu7kps97",
}
```

## Serialized Payload

```shell
ff0217010000000400020000000000000003b593aa66b53525c5399b4af5a4f583dede1c2a46176c6796a7284ee9c0a1167f0094357700000000000203037eaa8cb236c40a08fcb9d6220743ee6ae1b5c40e8a77a38f286516c3ff6639010301fd417566397113ba8c55de2f093a572744ed1829b37b56a129058000ef7bce0209d3c0f68994253cee24b23df3266ba1f0ca2f0666cd69a46544d63001cdf15016b09ed47ce05cc9096b87b73008f73aa51d4ddc8713d4ca0fcbcabeacfd738e0181201d55a92d5b2978d0c458684489cf133d18c521fbec431dd759d16bedbb004495d593cfb8be3293e2473acf504870d2dcf71dbee7620270e136ed63c5eef259099d225f7866178968f0c3581509d92d902914674c8f86b99eb55aaa97586e0171d86f3f6552b237dd81272a7b0da7718c4d26682255223dcf1928174082ce72b07218162938c674afe741119650135338eb3da159e0626ddab6b7851882e08b02d44d9bde77c9ea02d3516ab3263a77f4f9fbb90c30b47eba7a8bb87325edeb78dd69f914f28426e6ff661c4bc001f253130f4e7eb092a9131c8ca69dbfaff32f
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0400`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x03b593aa66b53525c5399b4af5a4f583dede1c2a46176c6796a7284ee9c0a1167f`    |
| **Fee:**                  | **[50]**  | **8**             | `0x0094357700000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Key Min:**              | **[59]**  | **1**             | `0x02`                |
| **Key Count:**            | **[60]**  | **1**             | `0x03`                |
| **Key 1:**                | **[61]**  | **33**            | `0x037eaa8cb236c40a08fcb9d6220743ee6ae1b5c40e8a77a38f286516c3ff663901`    |
| **Key 2:**                | **[94]**  | **33**            | `0x0301fd417566397113ba8c55de2f093a572744ed1829b37b56a129058000ef7bce`    |
| **Key 3:**                | **[127]** | **33**            | `0x0209d3c0f68994253cee24b23df3266ba1f0ca2f0666cd69a46544d63001cdf150`    |
| **Signature:**            | **[160]** | **64**            | `0x16b09ed47ce05cc9096b87b73008f73aa51d4ddc8713d4ca0fcbcabeacfd738e0181201d55a92d5b2978d0c458684489cf133d18c521fbec431dd759d16bedbb`  |
| **Signature 1 Flag:**     | **[224]** | **1**             | `0x00`  |
| **Signature 1:**          | **[225]** | **64**            | `0x4495d593cfb8be3293e2473acf504870d2dcf71dbee7620270e136ed63c5eef259099d225f7866178968f0c3581509d92d902914674c8f86b99eb55aaa97586e`  |
| **Signature 2 Flag:**     | **[289]** | **1**             | `0x01`  |
| **Signature 2:**          | **[290]** | **64**            | `0x71d86f3f6552b237dd81272a7b0da7718c4d26682255223dcf1928174082ce72b07218162938c674afe741119650135338eb3da159e0626ddab6b7851882e08b`  |
| **Signature 3 Flag:**     | **[354]** | **1**             | `0x02`  |
| **Signature 3:**          | **[355]** | **64**            | `0xd44d9bde77c9ea02d3516ab3263a77f4f9fbb90c30b47eba7a8bb87325edeb78dd69f914f28426e6ff661c4bc001f253130f4e7eb092a9131c8ca69dbfaff32f`  |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff0217010000000400020000000000000003b593aa66b53525c5399b4af5a4f583dede1c2a46176c6796a7284ee9c0a1167f0094357700000000000203037eaa8cb236c40a08fcb9d6220743ee6ae1b5c40e8a77a38f286516c3ff6639010301fd417566397113ba8c55de2f093a572744ed1829b37b56a129058000ef7bce0209d3c0f68994253cee24b23df3266ba1f0ca2f0666cd69a46544d63001cdf150004495d593cfb8be3293e2473acf504870d2dcf71dbee7620270e136ed63c5eef259099d225f7866178968f0c3581509d92d902914674c8f86b99eb55aaa97586e0171d86f3f6552b237dd81272a7b0da7718c4d26682255223dcf1928174082ce72b07218162938c674afe741119650135338eb3da159e0626ddab6b7851882e08b02d44d9bde77c9ea02d3516ab3263a77f4f9fbb90c30b47eba7a8bb87325edeb78dd69f914f28426e6ff661c4bc001f253130f4e7eb092a9131c8ca69dbfaff32f
```
