
# Type 3: Vote

## Signed Json Payload

```json
{
 "version": 2,
 "network": 23,
 "type": 3,
 "nonce": "2",
 "senderPublicKey": "02555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d",
 "fee": "100000000",
 "amount": "0",
 "asset": {
    "votes": [
        "+02555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d"
    ]
 },
 "signature": "77a40e4b4170ce613c8f9ccc0650887349330a9a8b459189ee379c88cf2c8506d65aa3ca8293705373f1bde8d6b27e5071de1785ac9c0182f41e364f8f9e3b64",
 "id": "fd59eaa4a2bbb3570c7b01ad464c968aa9bf73a40e0417c802ab30553ded8476"
}
```

## Serialized Payload

```shell
ff0217010000000300020000000000000002555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d00e1f5050000000000010102555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d77a40e4b4170ce613c8f9ccc0650887349330a9a8b459189ee379c88cf2c8506d65aa3ca8293705373f1bde8d6b27e5071de1785ac9c0182f41e364f8f9e3b64
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0300`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x02555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d`    |
| **Fee:**                  | **[50]**  | **8**             | `0x00e1f50500000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Number of Votes:**      | **[59]**  | **1**             | `0x01`                |
| **Vote:**                 | **[60]**  | **34**            | `0x0102555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d`  |
| **Signature:**            | **[94]**  | **64**            | `0x77a40e4b4170ce613c8f9ccc0650887349330a9a8b459189ee379c88cf2c8506d65aa3ca8293705373f1bde8d6b27e5071de1785ac9c0182f41e364f8f9e3b64`  |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff0217010000000300020000000000000002555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d00e1f5050000000000010102555806bca6737eaeaff6434d5171bac8aeb72533ed9bafb280dd11b328a3822d
```
