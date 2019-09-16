
# Type 9: HTLC Claim

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 9,
	"nonce": "2",
	"senderPublicKey": "0295b4261317ef9f27592fae7739438c49685fed351586dbc26a9cd871885a121a",
	"fee": "0",
	"amount": "0",
	"asset": {
		"claim": {
			"lockTransactionId": "09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c",
			"unlockSecret": "f5ea877a311ced90cf4524cb489e972f"
		}
	},
	"signature": "b05241178d54cdc625e4fc251dd939acbd2617295ea9dfd8282f1e3ccad8019718e278c10f291a25afb002ae253c139949d0fdbd6badb4b7616d70faf8c7b43e",
	"id": "af7702fd837c08ab0d63a7225a5549626afd209141fa3086f425797eb111350b"
}
```

## Serialized Payload

```shell
ff021701000000090002000000000000000295b4261317ef9f27592fae7739438c49685fed351586dbc26a9cd871885a121a00000000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c6635656138373761333131636564393063663435323463623438396539373266b05241178d54cdc625e4fc251dd939acbd2617295ea9dfd8282f1e3ccad8019718e278c10f291a25afb002ae253c139949d0fdbd6badb4b7616d70faf8c7b43e
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **TypeGroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0900`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x0295b4261317ef9f27592fae7739438c49685fed351586dbc26a9cd871885a121a`    |
| **Fee:**                  | **[50]**  | **8**             | `0x0000000000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Lock Id:**              | **[59]**  | **32**            | `0x09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c`  |
| **Unlock Secret:**        | **[91]**  | **32**            | `0x6635656138373761333131636564393063663435323463623438396539373266`  |
| **Signature:**            | **[123]**	| **64**            | `0xad7a61a76433260ef9dc687311ab6c657f6c733dbf1a80c3514da823d43226235a70a94fa1a0b8cb2f4b3d0be5011945bfbe8c8fc5b5ca0e07f6c2a37e3cf11b`  |

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff021701000000090002000000000000000295b4261317ef9f27592fae7739438c49685fed351586dbc26a9cd871885a121a00000000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c6635656138373761333131636564393063663435323463623438396539373266
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._
