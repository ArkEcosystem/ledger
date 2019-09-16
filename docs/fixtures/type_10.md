
# Type 10: HTLC Refund

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 10,
	"nonce": "3",
	"senderPublicKey": "02a53371b23f991740f968e3d96de42a67b4242e267cad8050ae4b68bf9ac20af2",
	"fee": "0",
	"amount": "0",
	"asset": {
		"refund": {
			"lockTransactionId": "09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c"
		}
	},
	"signature": "d3273eb5c13027f789939f999742d7a1d015e75f8ad29dc6482b84f320445c2cb29c3fd62168c1009d4ef000d7ad41d4295b755276c4eb3a604c7b07337f69cb",
	"id": "c77777da8e2ea747bacbab1c8e673e6ab32e5152be22eb6b1e0cfc9189908871",
}
```

## Serialized Payload

```shell
ff0217010000000a00030000000000000002a53371b23f991740f968e3d96de42a67b4242e267cad8050ae4b68bf9ac20af200000000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78cd3273eb5c13027f789939f999742d7a1d015e75f8ad29dc6482b84f320445c2cb29c3fd62168c1009d4ef000d7ad41d4295b755276c4eb3a604c7b07337f69cb
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
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x02a53371b23f991740f968e3d96de42a67b4242e267cad8050ae4b68bf9ac20af2`    |
| **Fee:**                  | **[50]**  | **8**             | `0x0000000000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Lock Id:**              | **[59]**  | **32**            | `0x09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c`  |
| **Signature:**            | **[91]**  | **64**            | `0xad7a61a76433260ef9dc687311ab6c657f6c733dbf1a80c3514da823d43226235a70a94fa1a0b8cb2f4b3d0be5011945bfbe8c8fc5b5ca0e07f6c2a37e3cf11b`  |

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff0217010000000a00030000000000000002a53371b23f991740f968e3d96de42a67b4242e267cad8050ae4b68bf9ac20af200000000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._
