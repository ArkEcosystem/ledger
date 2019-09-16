
# Type 0: Transfer

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 0,
	"nonce": "1",
	"senderPublicKey": "03a02b9d5fdd1307c2ee4652ba54d492d1fd11a7d1bb3f3a44c4a05e79f19de933",
	"fee": "10000000",
	"amount": "100000",
	"expiration": 0,
	"recipientId": "AJWRd23HNEhPLkK1ymMnwnDBX2a7QBZqff",
	"signature": "4f01bd21828a633a3c821b9984fe642deab87237b99e62a543ca6948ff1d6d32f2475ada1f933da0591c40603693614afa69fcb4caa2b4be018788de9f10c42a",
	"id": "8c85167cb1ca6f70e350f30173deb4c0a00ce7169d04f78fe4e4bcf2c3a75214"
}
```

## Serialized Payload

```shell
ff0217010000000000010000000000000003a02b9d5fdd1307c2ee4652ba54d492d1fd11a7d1bb3f3a44c4a05e79f19de933809698000000000000a08601000000000000000000171dfc69b54c7fe901e91d5a9ab78388645e2427ea4f01bd21828a633a3c821b9984fe642deab87237b99e62a543ca6948ff1d6d32f2475ada1f933da0591c40603693614afa69fcb4caa2b4be018788de9f10c42a
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0000`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0100000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x03a02b9d5fdd1307c2ee4652ba54d492d1fd11a7d1bb3f3a44c4a05e79f19de933`    |
| **Fee:**                  | **[50]**  | **8**             | `0x8096980000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x00`                |
| **Amount:**               | **[59]**  | **8**             | `0xa086010000000000`  |
| **Expiration:**           | **[67]**  | **4**             | `0x00000000`          |
| **Recipient:**            | **[71]**  | **21**            | `0x171dfc69b54c7fe901e91d5a9ab78388645e2427ea`    |
| **Signature:**            | **[92]**  | **64**            | `0x4f01bd21828a633a3c821b9984fe642deab87237b99e62a543ca6948ff1d6d32f2475ada1f933da0591c40603693614afa69fcb4caa2b4be018788de9f10c42a`  |

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff0217010000000000010000000000000003a02b9d5fdd1307c2ee4652ba54d492d1fd11a7d1bb3f3a44c4a05e79f19de933809698000000000000a08601000000000000000000171dfc69b54c7fe901e91d5a9ab78388645e2427ea
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._
