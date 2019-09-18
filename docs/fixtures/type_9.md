
# Type 9: HTLC Claim

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 9,
	"nonce": "3",
	"senderPublicKey": "039d974aa6feff6a19fde69a8a8b25b991798e98252765a887118ba61218f473a2",
	"fee": "0",
	"amount": "0",
	"asset": {
		"claim": {
			"lockTransactionId": "f84efeab77224af8959301a7185597a7cfbfbc9a4d99cb021af62f3714feb9d3",
			"unlockSecret": "f5ea877a311ced90cf4524cb489e972f"
		}
	},
	"signature": "c2b9f3655174c13686dde428cf18d5d18f465712985a7086b04860457e8d2db64443083bdf69fdc5b94dcd2c4c722606cf0e058ffae98d8f9f069177c5c189ab",
	"id": "d8acf49eba509e94494f454a86add1fab8b2130f223c9cc25e8e92745a584813"
}
```

## Serialized Payload

```shell
ff02170100000009000300000000000000039d974aa6feff6a19fde69a8a8b25b991798e98252765a887118ba61218f473a2000000000000000000f84efeab77224af8959301a7185597a7cfbfbc9a4d99cb021af62f3714feb9d36635656138373761333131636564393063663435323463623438396539373266c2b9f3655174c13686dde428cf18d5d18f465712985a7086b04860457e8d2db64443083bdf69fdc5b94dcd2c4c722606cf0e058ffae98d8f9f069177c5c189ab
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
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x039d974aa6feff6a19fde69a8a8b25b991798e98252765a887118ba61218f473a2`    |
| **Fee:**                  | **[50]**  | **8**             | `0x0000000000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Lock Id:**              | **[59]**  | **32**            | `0xf84efeab77224af8959301a7185597a7cfbfbc9a4d99cb021af62f3714feb9d3`  |
| **Unlock Secret:**        | **[91]**  | **32**            | `0x6635656138373761333131636564393063663435323463623438396539373266`  |
| **Signature:**            | **[123]**	| **64**            | `0xc2b9f3655174c13686dde428cf18d5d18f465712985a7086b04860457e8d2db64443083bdf69fdc5b94dcd2c4c722606cf0e058ffae98d8f9f069177c5c189ab`  |

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff02170100000009000300000000000000039d974aa6feff6a19fde69a8a8b25b991798e98252765a887118ba61218f473a2000000000000000000f84efeab77224af8959301a7185597a7cfbfbc9a4d99cb021af62f3714feb9d36635656138373761333131636564393063663435323463623438396539373266
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._
