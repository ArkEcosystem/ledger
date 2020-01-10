
# Type 10: HTLC Refund

## Signed Json Payload

```json
{
 "version": 2,
 "network": 23,
 "type": 10,
 "nonce": "3",
 "senderPublicKey": "037fc2e14f626586722a4f9e00dca2efbc4ac409c1ca63bc4309f56184265f95d5",
 "fee": "0",
 "amount": "0",
 "asset": {
    "refund": {
        "lockTransactionId": "c62bd36c162dd0116a08bf8a75cd6d1f83b8f5f1e17e89c8231ebb7af595f64d"
    }
 },
 "signature": "ae272f4650ee1d46260b8f62e7e956af33cd25587318fed056aec8e9d518e2394d0fd3166d8cd8506abfc303c644041a4bab35daf7c8aaa77f916ef09dc90336",
 "id": "4d9ee7f8b27999d4ce7acf6afee08e3da67bc1fca258f2bd17e426933a846602"
}
```

## Serialized Payload

```shell
ff0217010000000a000300000000000000037fc2e14f626586722a4f9e00dca2efbc4ac409c1ca63bc4309f56184265f95d5000000000000000000c62bd36c162dd0116a08bf8a75cd6d1f83b8f5f1e17e89c8231ebb7af595f64dae272f4650ee1d46260b8f62e7e956af33cd25587318fed056aec8e9d518e2394d0fd3166d8cd8506abfc303c644041a4bab35daf7c8aaa77f916ef09dc90336
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0a00`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0300000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x037fc2e14f626586722a4f9e00dca2efbc4ac409c1ca63bc4309f56184265f95d5`    |
| **Fee:**                  | **[50]**  | **8**             | `0x0000000000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Lock Id:**              | **[59]**  | **32**            | `0xc62bd36c162dd0116a08bf8a75cd6d1f83b8f5f1e17e89c8231ebb7af595f64d`  |
| **Signature:**            | **[91]**  | **64**            | `0xae272f4650ee1d46260b8f62e7e956af33cd25587318fed056aec8e9d518e2394d0fd3166d8cd8506abfc303c644041a4bab35daf7c8aaa77f916ef09dc90336`  |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff0217010000000a000300000000000000037fc2e14f626586722a4f9e00dca2efbc4ac409c1ca63bc4309f56184265f95d5000000000000000000c62bd36c162dd0116a08bf8a75cd6d1f83b8f5f1e17e89c8231ebb7af595f64d
```
