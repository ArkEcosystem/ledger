
# Type 7: Delegate Resignation

## Signed Json Payload

```json
{
 "version": 2,
 "network": 23,
 "type": 7,
 "nonce": "2",
 "senderPublicKey": "037a12518205254e6ebf25290d9786fd9821c43bb7319c9fc2499c8d472809dfaf",
 "fee": "2500000000",
 "amount": "0",
 "signature": "ad7a61a76433260ef9dc687311ab6c657f6c733dbf1a80c3514da823d43226235a70a94fa1a0b8cb2f4b3d0be5011945bfbe8c8fc5b5ca0e07f6c2a37e3cf11b",
 "id": "ee2a5253e28f66d5546b28bba96b4fa88973305e2e0d3b82afd5b3386ab0b6d4"
}
```

## Serialized Payload

```shell
ff02170100000007000200000000000000037a12518205254e6ebf25290d9786fd9821c43bb7319c9fc2499c8d472809dfaf00f902950000000000ad7a61a76433260ef9dc687311ab6c657f6c733dbf1a80c3514da823d43226235a70a94fa1a0b8cb2f4b3d0be5011945bfbe8c8fc5b5ca0e07f6c2a37e3cf11b
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0700`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x037a12518205254e6ebf25290d9786fd9821c43bb7319c9fc2499c8d472809dfaf`    |
| **Fee:**                  | **[50]**  | **8**             | `0x00f9029500000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Amount:**               | **[..]**  | **0**             |  Not Serialized       |
| **Signature:**            | **[59]**  | **64**            | `0xad7a61a76433260ef9dc687311ab6c657f6c733dbf1a80c3514da823d43226235a70a94fa1a0b8cb2f4b3d0be5011945bfbe8c8fc5b5ca0e07f6c2a37e3cf11b`  |

## Example Ledger Payload

```shell
python3 examples/example_helper.py --tx ff02170100000007000200000000000000037a12518205254e6ebf25290d9786fd9821c43bb7319c9fc2499c8d472809dfaf00f902950000000000
```
