
# Type 8: HTLC Lock

## Signed Json Payload

```json
{
    "version": 2,
    "network": 23,
    "type": 8,
    "nonce": "0",
    "senderPublicKey": "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192",
    "fee": "10000000",
    "amount": "1",
    "vendorField": "Hello World",
    "recipientId": "AZRJYXhknRmKMuwgqbYxr8FsfHQKn8Fk9K",
    "asset": {
        "lock": {
            "secretHash": "09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c",
            "expiration": {
                "type": 1,
                "value": 88506746
            }
        }
    },
    "signature": "3045022100a1cd1c89cb8e1accb0fb394dc5e14ee49385fb990c4b6ea60af37f0523213b950220037c0f8538c04d0041c3f8a3737a8e0fa7c59cde9d6ead9fad6101f0d5a85ea8",
    "id": "f981d2590e59d4ae8d563aa906899ebce0a56e93fde665fd7760152108bf7209",
    "serialized": "ff02170100000008000000000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19280969800000000000b48656c6c6f20576f726c64010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c017a81460517c18e4fdcd78bf9a9eac22e91d52edfeb04bc8e9e3045022100a1cd1c89cb8e1accb0fb394dc5e14ee49385fb990c4b6ea60af37f0523213b950220037c0f8538c04d0041c3f8a3737a8e0fa7c59cde9d6ead9fad6101f0d5a85ea8"
}
```

## Serialized Payload

```shell
ff02170100000008000000000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19280969800000000000b48656c6c6f20576f726c64010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c017a81460517c18e4fdcd78bf9a9eac22e91d52edfeb04bc8e9e3045022100a1cd1c89cb8e1accb0fb394dc5e14ee49385fb990c4b6ea60af37f0523213b950220037c0f8538c04d0041c3f8a3737a8e0fa7c59cde9d6ead9fad6101f0d5a85ea8
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_             |
| :--                       | :--:      | :---------------: | :----------------             |
| **Header:**               | **[0]**   | **1**             | `0xff`                        |
| **Version:**              | **[1]**   | **1**             | `0x02`                        |
| **Network:**              | **[2]**   | **1**             | `0x17`                        |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`                  |
| **Type:**                 | **[7]**   | **2**             | `0x0800`                      |
| **Nonce:**                | **[9]**   | **8**             | `0x0000000000000000`          |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192`    |
| **Fee:**                  | **[50]**  | **8**             | `0x8096980000000000`          |
| **VendorField Length:**   | **[58]**  | **1**             | `0x0b`                        |
| **VendorField**           | **[59]**  | **1**             | `0x48656c6c6f20576f726c64`    |
| **Amount:**               | **[70]**  | **8**             | `0x0100000000000000`          |
| **Secret Hash:**          | **[78]**  | **32**            | `0x09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c`  |
| **Expiration Type:**      | **[110]** | **1**             | `0x01`                        |
| **Expiration Value:**     | **[111]** | **4**             | `0x7a814605`                  |
| **Recipient:**            | **[115]** | **21**            | `0x177f2a95c7076ea278776d8fcecc5b18e588976da6`    |
| **Signature:**            | **[136]** | **71**            | `0x3045022100a1cd1c89cb8e1accb0fb394dc5e14ee49385fb990c4b6ea60af37f0523213b950220037c0f8538c04d0041c3f8a3737a8e0fa7c59cde9d6ead9fad6101f0d5a85ea8`    |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff02170100000008000000000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19280969800000000000b48656c6c6f20576f726c64010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c017a81460517c18e4fdcd78bf9a9eac22e91d52edfeb04bc8e9e
```
