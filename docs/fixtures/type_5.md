
# Type 5: IPFS

## Signed Json Payload

```json
{
 "version": 2,
 "network": 23,
 "type": 5,
 "nonce": "2",
 "senderPublicKey": "038e000c902d4551065ac5705637c685d52e6ac4032e158ad0370c5ef2bbafae2c",
 "fee": "500000000",
 "amount": "0",
 "asset": {
    "ipfs": "QmYSK2JyM3RyDyB52caZCTKFR3HKniEcMnNJYdk8DQ6KKB"
 },
 "signature": "ed8e729b40e73ab86c3b6675d463c19d88495bf4e091037e80352afe0ea29efff04d2667bfe8d78e5c4ad410fb0f7a0f511fbd657a54181aca8de4e8c6ebfe2c",
 "id": "77d8134144780e50db71adb496e02bcbde43e76a0cd7eeae7ff3641db75187ec",
}
```

## Serialized Payload

```shell
ff02170100000005000200000000000000038e000c902d4551065ac5705637c685d52e6ac4032e158ad0370c5ef2bbafae2c0065cd1d000000000012209608184d6cee2b9af8e6c2a46fc9318adf73329aeb8a86cf8472829fff5bb89eed8e729b40e73ab86c3b6675d463c19d88495bf4e091037e80352afe0ea29efff04d2667bfe8d78e5c4ad410fb0f7a0f511fbd657a54181aca8de4e8c6ebfe2c
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **TypeGroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0500`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x038e000c902d4551065ac5705637c685d52e6ac4032e158ad0370c5ef2bbafae2c`    |
| **Fee:**                  | **[50]**  | **8**             | `0x0065cd1d00000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **IPFS Hash:**            | **[59]**  | **34**            | `0x12209608184d6cee2b9af8e6c2a46fc9318adf73329aeb8a86cf8472829fff5bb89e`  |
| **Signature:**            | **[93]**  | **64**            | `0xed8e729b40e73ab86c3b6675d463c19d88495bf4e091037e80352afe0ea29efff04d2667bfe8d78e5c4ad410fb0f7a0f511fbd657a54181aca8de4e8c6ebfe2c`  |

## IPFS Hash Breakdown

| Item                      | Length                | Value                                                                     |
| :------------------------ | :----:                | :----                                                                     |
| **IPFS Hash**             | **46<br>_(chars)_**   | `QmYSK2JyM3RyDyB52caZCTKFR3HKniEcMnNJYdk8DQ6KKB`                          |
| **Decoded Base58**        | **34<br>_(bytes)_**   | `0x12209608184d6cee2b9af8e6c2a46fc9318adf73329aeb8a86cf8472829fff5bb89e`  |
| **Hash Type**             | **1<br>_(byte)_**     | `0x12`                                                                    |
| **Hash Length**           | **1<br>_(byte)_**     | `0x20`                                                                    |
| **32-Byte Hash**          | **32<br>_(bytes)_**   | `0x9608184d6cee2b9af8e6c2a46fc9318adf73329aeb8a86cf8472829fff5bb89e`      |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff02170100000005000200000000000000038e000c902d4551065ac5705637c685d52e6ac4032e158ad0370c5ef2bbafae2c0065cd1d000000000012209608184d6cee2b9af8e6c2a46fc9318adf73329aeb8a86cf8472829fff5bb89e
```
