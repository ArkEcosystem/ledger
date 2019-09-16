
# Type 8: HTLC Lock

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 8,
	"nonce": "2",
	"senderPublicKey": "0207ebc33a5f6eddf623706b6645b785eaa4405d14f80556461d8f78e0b1cb1884",
	"fee": "10000000",
	"amount": "1",
	"recipientId": "AZqDtF6WbksWaE2DSH6CVRV5kqvoCwnnnq",
	"asset": {
		"lock": {
			"secretHash": "09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c",
			"expiration": {
				"type": 1,
				"value": 78127967
			}
		}
	},
	"signature": "de19469ac62a66898814a1a2c9e396b826df1a1e1296191c66b10f09df412f7313d80c8c8d09a629094acc118fc42de048112921cd030b59c85536fc5471b05a",
	"id": "5dfcae6b5d954d05de58ef4d75693e515155e5fe8b08d44c78dffdf41e2c790e"
}
```

## Serialized Payload

```shell
ff021701000000080002000000000000000207ebc33a5f6eddf623706b6645b785eaa4405d14f80556461d8f78e0b1cb1884809698000000000000010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c015f23a8040000000017c61467acf99231ed0c717ca9c6bbf6fb44b1d138de19469ac62a66898814a1a2c9e396b826df1a1e1296191c66b10f09df412f7313d80c8c8d09a629094acc118fc42de048112921cd030b59c85536fc5471b05a
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
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x0207ebc33a5f6eddf623706b6645b785eaa4405d14f80556461d8f78e0b1cb1884`    |
| **Fee:**                  | **[50]**  | **8**             | `0x8096980000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Amount:**               | **[59]**  | **8**             | `0x0100000000000000`  |
| **Secret Hash:**          | **[67]**  | **32**            | `0x09b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c`  |
| **Expiration Type:**      | **[99]**  | **1**             | `0x01`                |
| **Expiration Value:**     | **[100]** | **8**             | `0x5f23a80400000000`  |
| **Recipient:**            | **[108]** | **21**            | `0x17c61467acf99231ed0c717ca9c6bbf6fb44b1d138`    |
| **Signature:**            | **[129]** | **64**            | `0xde19469ac62a66898814a1a2c9e396b826df1a1e1296191c66b10f09df412f7313d80c8c8d09a629094acc118fc42de048112921cd030b59c85536fc5471b05a`  |
> _htlc asset numbers are little-endian packed_

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff021701000000080002000000000000000207ebc33a5f6eddf623706b6645b785eaa4405d14f80556461d8f78e0b1cb1884809698000000000000010000000000000009b9a28393efd02fcd76a21b0f0f55ba2aad8f3640ff8cae86de033a9cfbd78c015f23a8040000000017c61467acf99231ed0c717ca9c6bbf6fb44b1d138
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._

