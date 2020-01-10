
# Type 8: HTLC Lock

## Signed Json Payload

```json
{
 "version": 2,
 "network": 23,
 "type": 8,
 "nonce": "2",
 "senderPublicKey": "020d272fab67c179a9e4df4d006344d3ca47fb531b4246b483373940f0603a9216",
 "fee": "10000000",
 "amount": "1",
 "recipientId": "ATNGUiu6sYRb7MXtdcVc7KjoyM6TdfuoC1",
 "asset": {
    "lock": {
        "secretHash": "09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c",
        "expiration": {
            "type": 1,
            "value": 78740307
        }
    }
 },
 "signature": "11b1c06b4e5ba7c196f6f36fca2540275173a472e61581e949cd24a7cf5ee98af6a74f3c919f9b82a2e65b51b737bdf22f7a08ffcf52b88dc4a16d6ac5c10bfe",
 "id": "f84efeab77224af8959301a7185597a7cfbfbc9a4d99cb021af62f3714feb9d3"
}
```

## Serialized Payload

```shell
ff02170100000008000200000000000000020d272fab67c179a9e4df4d006344d3ca47fb531b4246b483373940f0603a9216809698000000000000010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c01537bb104177f2a95c7076ea278776d8fcecc5b18e588976da611b1c06b4e5ba7c196f6f36fca2540275173a472e61581e949cd24a7cf5ee98af6a74f3c919f9b82a2e65b51b737bdf22f7a08ffcf52b88dc4a16d6ac5c10bfe
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0800`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x020d272fab67c179a9e4df4d006344d3ca47fb531b4246b483373940f0603a9216`    |
| **Fee:**                  | **[50]**  | **8**             | `0x8096980000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Amount:**               | **[59]**  | **8**             | `0x0100000000000000`  |
| **Secret Hash:**          | **[67]**  | **32**            | `0x09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c`  |
| **Expiration Type:**      | **[99]**  | **1**             | `0x01`                |
| **Expiration Value:**     | **[100]** | **4**             | `0x537bb104`          |
| **Recipient:**            | **[104]** | **21**            | `0x177f2a95c7076ea278776d8fcecc5b18e588976da6`    |
| **Signature:**            | **[125]** | **64**            | `0x11b1c06b4e5ba7c196f6f36fca2540275173a472e61581e949cd24a7cf5ee98af6a74f3c919f9b82a2e65b51b737bdf22f7a08ffcf52b88dc4a16d6ac5c10bfe`  |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff02170100000008000200000000000000020d272fab67c179a9e4df4d006344d3ca47fb531b4246b483373940f0603a9216809698000000000000010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c01537bb104177f2a95c7076ea278776d8fcecc5b18e588976da6
```
